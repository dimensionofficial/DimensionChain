The following steps must be taken for the example script to work.

0. Create wallet
0. Create account for eonio.token
0. Create account for scott
0. Create account for exchange
0. Set token contract on eonio.token
0. Create EOS token
0. Issue initial tokens to scott

**Note**:
Deleting the `transactions.txt` file will prevent replay from working.


### Create wallet
`cleon wallet create`

### Create account steps
`cleon create key`

`cleon create key`

`cleon wallet import  --private-key <private key from step 1>`

`cleon wallet import  --private-key <private key from step 2>`

`cleon create account eonio <account_name> <public key from step 1> <public key from step 2>`

### Set contract steps
`cleon set contract eonio.token /contracts/eonio.token -p eonio.token@active`

### Create EOS token steps
`cleon push action eonio.token create '{"issuer": "eonio.token", "maximum_supply": "100000.0000 EOS", "can_freeze": 1, "can_recall": 1, "can_whitelist": 1}' -p eonio.token@active`

### Issue token steps
`cleon push action eonio.token issue '{"to": "scott", "quantity": "900.0000 EOS", "memo": "testing"}' -p eonio.token@active`
