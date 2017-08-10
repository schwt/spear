#include "struct.h"

class CSpear
{
public:
    CSpear();
    ~CSpear();

public:
    bool Init(const char* cstr_config_file);
    bool Calc();

private:
    bool ReadConfigFile(const char* cstr_config_file, const char* cstr_log_file);
private:
    CMyLogMessages cls_log_msg;         // 日志对象

    //////////////////////////// new ///////////////////////////////
    // files
    string F_train_data_;
    string F_score_item_;
    string F_score_user_;
    string F_matrix_ivt_user_;
    string F_matrix_ivt_item_;
    string F_top_user_;
    string F_top_item_;

    // if not use disk_cache
    vector<DataNode> vec_data_;
    vector<MatrixInvert> vec_matrix_ivt_item_;
    vector<MatrixInvert> vec_matrix_ivt_user_;

    vector<MatrixIndex> vec_matrix_idx_user_;    // A_ij索引 vec[cid]->pid,score
    vector<MatrixIndex> vec_matrix_idx_item_;    // A_ji索引 vec[pid]->cid,score
    vector<float> vec_score_user_;
    vector<float> vec_score_item_;
    hash_map<int, int> hm_uid_;
    hash_map<int, int> hm_pid_;
    vector<int> vec_uid_;
    vector<int> vec_pid_;
    size_t num_user_;
    size_t num_item_;
    int iteration_;
    float stop_err_;
    int top_num_;

    //////////////////////////// Defined in config.ini ///////////////////////////////
    int BUFFERCNT;
    int SORTMEMSIZE;
    int MAX_EXPERT;
    int DISK_CACHE;
    
///////////////////////////////////////////////////////////

private:
    bool SourceDataManage();

    bool LoadData();
    bool LoadData(const string&);
    bool MakeMatrixP2U(const string&);
    bool MakeMatrixU2P(const string&);
    bool Train();
    bool WriteScore();
    bool WriteTopUser();
    bool WriteTopItem();
    float calc_rse(const vector<float>&, const vector<float>&);
};


