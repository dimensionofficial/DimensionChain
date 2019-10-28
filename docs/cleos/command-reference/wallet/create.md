## Description
Creates a wallet with the specified name. If no name is given, the wallet will be created with the name 'default'.

## Commands

```shell
$ ./cleos wallet create
or
$ ./cleos wallet create -n second-wallet
```

## Outputs


```shell
Creating wallet: default
Save password to use in the future to unlock this wallet.
Without password imported keys will not be retrievable.
"PW5JD9cw9YY288AXPvnbwUk5JK4Cy6YyZ83wzHcshu8F2akU9rRWE"
or
Creating wallet: second-wallet
Save password to use in the future to unlock this wallet.
Without password imported keys will not be retrievable.
"PW5Ji6JUrLjhKAVn68nmacLxwhvtqUAV18J7iycZppsPKeoGGgBEw"
```
## Positionals

None

## Options
- `-n, --name` _TEXT_ - The name of the new wallet
- `-f, --file` _TEXT_ - Name of file to write wallet password output to. (Must be set, unless "--to-console" is passed
- `--to-console` - Print password to console