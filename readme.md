# SPEAR

## Algorithm
Rating each user and item by iterating a behavior matrix.
 
- user score: $\mathbf e = (e_1, \cdots, e_u, \cdots, e_M)^\top \in \mathbb R^M $
- item score: $\mathbf q = (q_1, \cdots, q_u, \cdots, q_N)^\top \in \mathbb R^N $
- behavior matrix: $A = {A_{ui}} \in \mathbb R^{M\times N}$
- in iteration $t$:
$$
\mathbf e^{(t)} = \frac 1 Z A \  \mathbf q^{(t-1)} \\
\mathbf q^{(t)} = \frac 1 Z A^\top \  \mathbf e^{(t)}
$$
where $Z$ is for normalization. In this implementation, $Z$ takes the max value (inducing max score tobe 1).

## usage

##### 1. configure
configure in file `./config.ini`
- `iteration`: max num of iteration for train
- `stop_err`: threthold value of RSE difference of two adjacent iterations to stop train
- `top_num`: number of top users(Experts) and top items(high quality items) to output

##### 2. input
training data file is setted in config file: `file: train_data`
format:  `uid pid score`

##### 3. output
user score: `file: user_score`
item score: `file: item_score`
top users(Expert): `file: top_user`
top items: `file: top_item`


##### 4. run
`cd ./src/`
generate sample training set for demonstration:
`./generate_data.py`
run the training process:
`./spear config.ini`

## reference
[SPEAR: SPAMMING-RESISTANT EXPERTISE ANALYSIS AND RANKING IN COLLABORATIVE TAGGING SYSTEMS](http://onlinelibrary.wiley.com/doi/10.1111/j.1467-8640.2011.00384.x/citedby)


