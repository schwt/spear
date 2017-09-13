
#include <limits.h>
#include "spear.h"
// #include "utils.hpp"

#define Min(a, b) ( (a)<(b) ? (a):(b) )
#define Max(a, b) ( (a)>(b) ? (a):(b) )

bool CMP_BY_PSU(const DataNode& left, const DataNode& right) {
    return ((left.item_id  < right.item_id) ||
            (left.item_id == right.item_id  &&  left.score  < right.score) ||
            (left.item_id == right.item_id  &&  left.score == right.score  && left.user_id < right.user_id));
}
bool CMP_BY_USP(const DataNode& left, const DataNode& right) {
    return ((left.user_id  < right.user_id) ||
            (left.user_id == right.user_id  &&  left.score  < right.score) ||
            (left.user_id == right.user_id  &&  left.score == right.score  && left.item_id < right.item_id));
}

CSpear::CSpear() {
    F_matrix_ivt_user_ = "../data/temp/matrix_user.ivt";
    F_matrix_ivt_item_ = "../data/temp/matrix_item.ivt";
}

CSpear::~CSpear()
{
}

// 初始化
bool CSpear::Init(const char* s_f_config)
{
    cout << "config file path: " << s_f_config << endl;
    bool res = true;

    if (!cls_log_msg.SetLogFile("./log.txt", "Spear")) return false;
    if (!ReadConfigFile(s_f_config, "./log.txt"))  {
        cls_log_msg.log(__LINE__, false, "ReadConfigFile");
        return false;
    }
    else {
        cls_log_msg.log(__LINE__, true, "ReadConfigFile");
    }
    return res;
}

//************* 读配置文件 *************//
bool CSpear::ReadConfigFile(const char* s_f_config, const char* s_f_log)
{
    cls_log_msg.log("<<<<<<<<<<<<<<<ReadConfigFile>>>>>>>>>>>>>>>");

    Config ReadConfig(s_f_config);

    bool res = true;

    if (!(res = ReadConfig.ReadInto("file", "train_data",  F_train_data_))) return res;
    // 输出
    if (!(res = ReadConfig.ReadInto("file", "user_score",  F_score_user_))) return res;
    if (!(res = ReadConfig.ReadInto("file", "item_score",  F_score_item_))) return res;
    if (!(res = ReadConfig.ReadInto("file", "top_user",  F_top_user_))) return res;
    if (!(res = ReadConfig.ReadInto("file", "top_item",  F_top_item_))) return res;
    
    if (!(res = ReadConfig.ReadInto("data", "BUFFERCNT", BUFFERCNT))) return res;
    if (!(res = ReadConfig.ReadInto("data", "SORTMEMSIZE", SORTMEMSIZE))) return res;

    if (!(res = ReadConfig.ReadInto("data", "iteration", iteration_))) return res;
    if (!(res = ReadConfig.ReadInto("data", "stop_err", stop_err_))) return res;
    if (!(res = ReadConfig.ReadInto("data", "top_num", top_num_))) return res;
    
    return res;
}

/////////// 主函数 /////////////
bool CSpear::Calc()
{
    bool res = true;

    res = SourceDataManage();
    cls_log_msg.log(__LINE__, res, "SourceDataManage");
    if (!res) return res;

    res = Train();
    cls_log_msg.log(__LINE__, res, "Train");
    if (!res) return res;

    res = WriteScore();
    cls_log_msg.log(__LINE__, res, "WriteScore");
    if (!res) return res;

    res = WriteTopUser();
    cls_log_msg.log(__LINE__, res, "WriteTopUser");
    if (!res) return res;

    res = WriteTopItem();
    cls_log_msg.log(__LINE__, res, "WriteTopItem");
    if (!res) return res;

    cout << endl;
    // RemoveFile(F_matrix_ivt_item_);
    // RemoveFile(F_matrix_ivt_user_);
    return res;
}

//////////// 数据预处理 ////////////
bool CSpear::SourceDataManage()
{
    cls_log_msg.log("==================SourceDataManage==================");
    bool res = true;
    
    
    string f_temp_data        = "../data/temp/data.dat";
    string f_temp_data_sorted = "../data/temp/data.sorted.dat";

    res = LoadData(f_temp_data);
    cls_log_msg.log(__LINE__, res, "Loaddata");
    if (!res) return res;

    cout << "\nSorting by pid/score/uid..." << endl;
    if(K_MergeFile<DataNode>(f_temp_data.c_str(),
                             f_temp_data_sorted.c_str(), CMP_BY_PSU,
                             SORTMEMSIZE) == -1) res = false;
    cls_log_msg.log(__LINE__, res, "K_MergeFile " + f_temp_data 
                                  + " to " + f_temp_data_sorted);
    if (!res) return res;


    res = MakeMatrixP2U(f_temp_data_sorted);
    cls_log_msg.log(__LINE__, res, "MakeMatrixP2U");
    if (!res) return res;

    cout << endl;
    RemoveFile(f_temp_data_sorted);
 
    cout << "\nSorting by uid/score/pid..." << endl;
    if(K_MergeFile<DataNode>(f_temp_data.c_str(),
                             f_temp_data_sorted.c_str(),
                             CMP_BY_USP, SORTMEMSIZE) == -1) res = false;
    cls_log_msg.log(__LINE__, res, "K_MergeFile " + f_temp_data
                                  + " to " + f_temp_data_sorted);
    if (!res) return res;


    res = MakeMatrixU2P(f_temp_data_sorted);
    cls_log_msg.log(__LINE__, res, "MakeMatrixU2P");
    if (!res) return res;

    cout << endl;
    RemoveFile(f_temp_data);
    RemoveFile(f_temp_data_sorted);

    return res;
}

// input text: "uid pid score"
// output bin: DataNode
bool CSpear::LoadData(const string& dst) {
    cls_log_msg.log("-------------LoadData-------------");
    FILE *fp_dst = fopen(dst.c_str(), "wb");
    if (!fp_dst) {
        printf("error open file %s!\n", dst.c_str());
        return false;
    }
    ifstream fin(F_train_data_.c_str());
    string line;
    vector<string> vec;
    vector<DataNode> buff;
    buff.resize(BUFFERCNT);
    int idc = 0;
    int cnt = 0;
    int mapped_uid = 0;
    int mapped_pid = 0;
    hash_map<int,int>::iterator ith;

    while (getline (fin, line)) {
        stringUtils::split(line, " ", vec);
        // my_split(line, " ", vec);
        // printf("[%s]\n", line.c_str());
        // for (size_t i = 0; i < vec.size(); i++) {
        //     printf("(%s)", vec[i].c_str());
        // }
        // printf("\n");
        // getchar();
        if (vec.size() != 3) continue;
        int user_id = atoi(vec[0].c_str());
        int item_id = atoi(vec[1].c_str());

        ith = hm_uid_.find(user_id);
        if (ith == hm_uid_.end()) {
            mapped_uid = hm_uid_.size();
            hm_uid_.insert(make_pair(user_id, mapped_uid));
        } else
            mapped_uid = ith->second;
        ith = hm_pid_.find(item_id);
        if (ith == hm_pid_.end()) {
            mapped_pid = hm_pid_.size();
            hm_pid_.insert(make_pair(item_id, mapped_pid));
        } else
            mapped_pid = ith->second;

        buff[idc].user_id = mapped_uid;
        buff[idc].item_id = mapped_pid;
        buff[idc].score = atof(vec[1].c_str());
        cnt++;

        if (++idc >= BUFFERCNT) {
            fwrite(&buff[0], sizeof(DataNode), idc, fp_dst);
            idc = 0;
        }
    }
    if (idc > 0) {
        fwrite(&buff[0], sizeof(DataNode), idc, fp_dst);
    }
    fclose(fp_dst);
    num_item_ = hm_pid_.size();
    num_user_ = hm_uid_.size();

    vec_uid_.resize(num_user_);
    vec_pid_.resize(num_item_);
    for (hash_map<int,int>::iterator itm = hm_uid_.begin(); itm != hm_uid_.end(); itm++) {
        vec_uid_[itm->second] = itm->first;
    }
    for (hash_map<int,int>::iterator itm = hm_pid_.begin(); itm != hm_pid_.end(); itm++) {
        vec_pid_[itm->second] = itm->first;
    }

    printf("# user count: %ld\n", num_user_);
    printf("# item count: %ld\n", num_item_);
    printf("# input data: %d\n", cnt);
    return true;
}

// 生成矩阵 A(i,j)的倒排索引: i => j
// input  bin: DataNode(sorted)
// output bin: idx, ivt
bool CSpear::MakeMatrixU2P(const string& f_src) {
    cls_log_msg.log("-------------MakeMatrixU2P-------------");
    FILE* fp_src = fopen(f_src.c_str(), "rb");
    FILE* fp_ivt= fopen(F_matrix_ivt_item_.c_str(), "wb");
    int from = 0;
    struct DataNode     node;
    struct MatrixIndex   str_index;
    struct MatrixInvert  str_invert;
    vector<MatrixInvert> vec_invert_buf;
    str_index.count  = 0;
    str_index.offset = 0;
    vec_matrix_idx_user_.resize(num_user_, str_index);

    fread(&node, sizeof(DataNode), 1, fp_src);
    int uid_old      = node.user_id;
    str_invert.id    = node.item_id;
    str_invert.score = node.score;
    vec_invert_buf.push_back(str_invert);
    struct DataNode* readbuf = new struct DataNode[BUFFERCNT];
    if (!CheckMemAlloc(readbuf, BUFFERCNT)) {
        printf("error allocate mem!\n");
        return false;
    }

    while(!feof(fp_src))
    {
        int size = fread(readbuf, sizeof(DataNode), BUFFERCNT, fp_src);

        for (int i = 0; i < size; ++i) {
            if (uid_old != readbuf[i].user_id) {
                str_index.count  = (int)vec_invert_buf.size();
                str_index.offset = (long long)from * sizeof(MatrixInvert);

                vec_matrix_idx_user_[uid_old] = str_index;
                fwrite(&vec_invert_buf[0], sizeof(MatrixInvert), str_index.count, fp_ivt);

                vec_invert_buf.clear();
                uid_old = readbuf[i].user_id;
                from   += str_index.count;
            }
            str_invert.id    = readbuf[i].item_id;
            str_invert.score = readbuf[i].score;
            vec_invert_buf.push_back(str_invert);
        }
        if (size < BUFFERCNT) break; 
        memset(readbuf, 0, BUFFERCNT);
    }
    if (vec_invert_buf.size() > 0) {
        str_index.count  = (int)vec_invert_buf.size();
        str_index.offset = (long long)from * sizeof(MatrixInvert);
        vec_matrix_idx_user_[uid_old] = str_index;
        fwrite(&vec_invert_buf[0], sizeof(MatrixInvert),
               str_index.count, fp_ivt);
    }
    delete []readbuf;
    readbuf = NULL;
    fclose(fp_src);
    fclose(fp_ivt);
    bool res = IdxIvtCheck<MatrixIndex, MatrixInvert>(vec_matrix_idx_user_, F_matrix_ivt_item_);
    if (!res) printf("Check Invert: Error!\n");

    return res;
}

// 生成矩阵 A(i,j)的倒排索引: j => i
// input  bin: DataNode(sorted)
// output bin: idx, ivt
bool CSpear::MakeMatrixP2U(const string& f_src) {
    cls_log_msg.log("-------------MakeMatrixP2U-------------");
    bool res = false;
    FILE* fp_src = fopen(f_src.c_str(), "rb");
    FILE* fp_ivt= fopen(F_matrix_ivt_user_.c_str(), "wb");
    int from = 0;
    struct DataNode     node;
    struct MatrixIndex   str_index;
    struct MatrixInvert  str_invert;
    vector<MatrixInvert> vec_invert_buf;
    str_index.count  = 0;
    str_index.offset = 0;
    vec_matrix_idx_item_.resize(num_item_, str_index);

    fread(&node, sizeof(DataNode), 1, fp_src);
    int pid_old          = node.item_id;
    str_invert.id    = node.user_id;
    str_invert.score = node.score;
    vec_invert_buf.push_back(str_invert);
    struct DataNode* readbuf = new struct DataNode[BUFFERCNT];
    if (!CheckMemAlloc(readbuf, BUFFERCNT)) {
        printf("error allocate mem!\n");
        return false;
    }

    while(!feof(fp_src))
    {
        int size = fread(readbuf, sizeof(DataNode), BUFFERCNT, fp_src);

        for (int i = 0; i < size; ++i) {
            if (pid_old != readbuf[i].item_id) {
                str_index.count  = (int)vec_invert_buf.size();
                str_index.offset = (long long)from * sizeof(MatrixInvert);

                vec_matrix_idx_item_[pid_old] = str_index;
                fwrite(&vec_invert_buf[0], sizeof(MatrixInvert), str_index.count, fp_ivt);

                vec_invert_buf.clear();
                pid_old = readbuf[i].item_id;
                from   += str_index.count;
            }
            str_invert.id    = readbuf[i].user_id;
            str_invert.score = readbuf[i].score;
            vec_invert_buf.push_back(str_invert);
        }
        if (size < BUFFERCNT) break; 
        memset(readbuf, 0, BUFFERCNT);
    }
    if (vec_invert_buf.size() > 0)
    {
        str_index.count  = (int)vec_invert_buf.size();
        str_index.offset = (long long)from * sizeof(MatrixInvert);
        vec_matrix_idx_item_[pid_old] = str_index;
        fwrite(&vec_invert_buf[0], sizeof(MatrixInvert),
               str_index.count, fp_ivt);
    }
    delete []readbuf;
    readbuf = NULL;
    fclose(fp_src);
    fclose(fp_ivt);
    res = IdxIvtCheck<MatrixIndex, MatrixInvert>(vec_matrix_idx_item_, F_matrix_ivt_user_);
    if (!res) printf("Check Invert: Error!\n");

    return res;
}

bool CSpear::Train() {
    cls_log_msg.log("-------------Train-------------");
    FILE* fp_ivt_item =  fopen(F_matrix_ivt_item_.c_str(),  "rb");
    FILE* fp_ivt_user =  fopen(F_matrix_ivt_user_.c_str(),  "rb");
    if (!fp_ivt_item || !fp_ivt_user) { printf("ERROR open file!\n"); return false;}
     
    vector<MatrixInvert> buff_ivt;
    vector<MatrixInvert>::iterator it_ivt;
    vector<float> vec_score_user_old;

    vec_score_user_.resize(num_user_, 1.0);
    vec_score_item_.resize(num_item_, 1.0);
    vec_score_user_old.resize(num_user_, 1.0);

    float RSE_old = 0.;
    for (int iter = 0; iter < iteration_; iter++) {
        double max = INT_MIN;
        for (size_t uid = 0; uid < num_user_; uid++) {
            ReadInvert(fp_ivt_item, vec_matrix_idx_user_[uid], buff_ivt);
            vec_score_user_[uid] = 0.;
            for (it_ivt = buff_ivt.begin(); it_ivt != buff_ivt.end(); it_ivt++) {
                vec_score_user_[uid] +=  it_ivt->score * vec_score_item_[it_ivt->id];
            }
            max = Max(max, vec_score_user_[uid]);
        }
        for (size_t uid = 0; uid < num_user_; uid++) {
            vec_score_user_[uid] = vec_score_user_[uid] / max;
        }
        max = INT_MIN;
        for (size_t pid = 0; pid < num_item_; pid++) {
            ReadInvert(fp_ivt_user, vec_matrix_idx_item_[pid], buff_ivt);
            vec_score_item_[pid] = 0.;
            for (it_ivt = buff_ivt.begin(); it_ivt != buff_ivt.end(); it_ivt++) {
                vec_score_item_[pid] +=  it_ivt->score * vec_score_user_[it_ivt->id];
            }
            max = Max(max, vec_score_item_[pid]);
        }
        for (size_t pid = 0; pid < num_item_; pid++) {
            vec_score_item_[pid] = vec_score_item_[pid] / max;
        }
        // print
        double RSE = calc_rse(vec_score_user_, vec_score_user_old);
        printf("[%d] %.10f   (%.10f %.10f) \n", iter+1, RSE, vec_score_user_[0], vec_score_user_[num_user_-1]);
        if (fabs(RSE - RSE_old) < stop_err_) break;
        RSE_old = RSE;
        for (size_t uid = 0; uid < num_user_; uid++) {
            vec_score_user_old[uid] = vec_score_user_[uid];
        }
    }
    fclose(fp_ivt_item);
    fclose(fp_ivt_user);
    return true;
}

float CSpear::calc_rse(const vector<float>& vec1, const vector<float>& vec2) {
    if (vec1.size() != vec2.size()) return -1.0;
    double sum = 0.0;
    for (size_t i = 0; i < vec1.size(); i++) {
        sum += (vec1[i] - vec2[i]) * (vec1[i] - vec2[i]);
    }
    return sqrt(sum);
}

bool CSpear::WriteScore() {
    cls_log_msg.log("-------------WriteScore-------------");
    FILE *fp_user = fopen(F_score_user_.c_str(), "w");
    FILE *fp_item = fopen(F_score_item_.c_str(), "w");
    if (!fp_user || !fp_item) {
        printf("ERROR open files!\n");
        return false;
    }
    vector<tNode> vec_buff(num_user_);
    for (size_t uid = 0; uid < num_user_; uid++)
        fprintf(fp_user, "%d\t%f\n", vec_uid_[uid], vec_score_user_[uid]);
    for (size_t pid = 0; pid < num_item_; pid++)
        fprintf(fp_item, "%d\t%f\n", vec_pid_[pid], vec_score_item_[pid]);
    fclose(fp_user);
    fclose(fp_item);
    return true;
}

bool CSpear::WriteTopUser() {
    cls_log_msg.log("-------------WriteTopUser-------------");
    
    vector<tNode> vec_buff;
    vec_buff.resize(num_user_);
    for (size_t i = 0; i < num_user_; i++) {
        vec_buff[i].id = vec_uid_[i];
        vec_buff[i].score = vec_score_user_[i];
    }
    sort(vec_buff.begin(), vec_buff.end());

    FILE* fp_dst = fopen(F_top_user_.c_str(), "w");
    if (!fp_dst) {
        printf("ERROR open file: %s\n", F_top_user_.c_str());
        return false;
    }
    for (size_t i = 0; i < Min(top_num_, num_user_); i++) {
        fprintf(fp_dst, "%d\t%.10f\n", vec_buff[i].id, vec_buff[i].score);
    }
    fclose(fp_dst);
    return true;
}

bool CSpear::WriteTopItem() {
    cls_log_msg.log("-------------WriteTopItem-------------");
    
    vector<tNode> vec_buff;
    vec_buff.resize(num_item_);
    for (size_t i = 0; i < num_item_; i++) {
        vec_buff[i].id = vec_pid_[i];
        vec_buff[i].score = vec_score_item_[i];
    }
    sort(vec_buff.begin(), vec_buff.end());

    FILE* fp_dst = fopen(F_top_item_.c_str(), "w");
    if (!fp_dst) {
        printf("ERROR open file: %s\n", F_top_item_.c_str());
        return false;
    }
    for (size_t i = 0; i < Min(top_num_, num_item_); i++) {
        fprintf(fp_dst, "%d\t%.10f\n", vec_buff[i].id, vec_buff[i].score);
    }
    fclose(fp_dst);
    return true;
}


