#!/usr/bin/env python3

from testUtils import Utils
import testUtils
import time
from Cluster import Cluster
from core_symbol import CORE_SYMBOL
from WalletMgr import WalletMgr
from Node import BlockType
from Node import Node
from TestHelper import TestHelper
from TestHelper import AppArgs

import decimal
import json
import math
import re
import signal

###############################################################
# ship_test
# 
# This test sets up <-p> producing node(s) and <-n - -p>
#   non-producing node(s). One of the non-producing nodes
#   is configured with the state_history_plugin.  An instance
#   of node will be started with a client javascript to exercise
#   the SHiP API.
#
###############################################################

Print=Utils.Print

from core_symbol import CORE_SYMBOL

appArgs = AppArgs()
minTotalAccounts = 5
args = TestHelper.parse_args({"-p", "-n","--dump-error-details","--keep-logs","-v","--leave-running","--clean-run"}, applicationSpecificArgs=appArgs)

Utils.Debug=args.v
totalProducerNodes=args.p
totalNodes=args.n
if totalNodes<=totalProducerNodes:
    totalNodes=totalProducerNodes+1
totalNonProducerNodes=totalNodes-totalProducerNodes
totalProducers=totalProducerNodes
cluster=Cluster(walletd=True)
dumpErrorDetails=args.dump_error_details
keepLogs=args.keep_logs
dontKill=args.leave_running
killAll=args.clean_run
walletPort=TestHelper.DEFAULT_WALLET_PORT

walletMgr=WalletMgr(True, port=walletPort)
testSuccessful=False
killEosInstances=not dontKill
killWallet=not dontKill

WalletdName=Utils.EosWalletName
ClientName="cleos"

try:
    TestHelper.printSystemInfo("BEGIN")

    cluster.setWalletMgr(walletMgr)
    cluster.killall(allInstances=killAll)
    cluster.cleanup()
    Print("Stand up cluster")
    specificExtraNodeosArgs={}
    # producer nodes will be mapped to 0 through totalProducerNodes-1, so the number totalProducerNodes will be the non-producing node
    specificExtraNodeosArgs[totalProducerNodes]="--plugin eosio::state_history_plugin --disable-replay-opts"

    if cluster.launch(pnodes=totalProducerNodes,
                      totalNodes=totalNodes, totalProducers=totalProducers,
                      useBiosBootFile=False, specificExtraNodeosArgs=specificExtraNodeosArgs) is False:
        Utils.cmdError("launcher")
        Utils.errorExit("Failed to stand up eos cluster.")

    # ***   create accounts to vote in desired producers   ***

    Print("creating accounts")
    accounts=Cluster.createAccountKeys(3)
    if accounts is None:
        Utils.errorExit("FAILURE - create keys")
    accounts[0].name = "tester111111"
    accounts[1].name = "tester111112"
    accounts[2].name = "tester111113"

    accountsToCreate = [cluster.eosioAccount]
    for account in accounts:
        accountsToCreate.append(account)

    testWalletName="test"

    Print("Creating wallet \"%s\"." % (testWalletName))
    testWallet=walletMgr.create(testWalletName, accountsToCreate)

    for _, account in cluster.defProducerAccounts.items():
        walletMgr.importKey(account, testWallet, ignoreDupKeyWarning=True)

    Print("Wallet \"%s\" password=%s." % (testWalletName, testWallet.password.encode("utf-8")))

    for account in accounts:
        walletMgr.importKey(account, testWallet)

    # ***   identify each node (producers and non-producing node)   ***

    nonProdNodes=[]
    prodNodes=[]
    for i in range(0, totalNodes):
        node=cluster.getNode(i)
        nodeProducers=Cluster.parseProducers(i)
        numProducers=len(nodeProducers)
        Print("node has producers=%s" % (nodeProducers))
        if numProducers==0:
            nonProdNodes.append(node)
        else:
            prodNodes.append(node)
    nonProdNodeCount = len(nonProdNodes)

    # ***   delegate bandwidth to accounts   ***

    node=nonProdNodes[0]
    startTime = time.perf_counter()
    Print("Create new accounts via %s" % (cluster.eosioAccount.name))
    # create accounts via eosio as otherwise a bid is needed
    for account in accounts:
        node.createInitializeAccount(account, cluster.eosioAccount, stakedDeposit=0, waitForTransBlock=True, stakeNet=1000, stakeCPU=1000, buyRAM=1000, exitOnError=True)

    nextTime = time.perf_counter()
    Print("Create new accounts took %s sec" % (nextTime - startTime))
    startTime = nextTime

    Print("Transfer funds to new accounts via %s" % (cluster.eosioAccount.name))
    for account in accounts:
        transferAmount="1000.0000 {0}".format(CORE_SYMBOL)
        Print("Transfer funds %s from account %s to %s" % (transferAmount, cluster.eosioAccount.name, account.name))
        node.transferFunds(cluster.eosioAccount, account, transferAmount, "test transfer", waitForTransBlock=True)

    nextTime = time.perf_counter()
    Print("Transfer funds took %s sec" % (nextTime - startTime))
    startTime = nextTime

    Print("Delegate Bandwidth to new accounts")
    for account in accounts:
        node.delegatebw(account, 200.0000, 200.0000, waitForTransBlock=True, exitOnError=True)

    nextTime = time.perf_counter()
    Print("Delegate Bandwidth took %s sec" % (nextTime - startTime))
    startTime = nextTime

    #verify nodes are in sync and advancing
    cluster.waitOnClusterSync(blockAdvancing=5)

    cmd = "node tests/ship_client.js"
    if Utils.Debug: Utils.Print("cmd: %s" % (cmd))
    start=time.perf_counter()
    rtn=Utils.runCmdReturnJson(cmd, silentErrors=False)
    if Utils.Debug:
        end=time.perf_counter()
        Utils.Print("cmd Duration: %.3f sec" % (end-start))
        printReturn=json.dumps(rtn)
        Utils.Print("cmd returned: %s" % (printReturn))

    testSuccessful = True
finally:
    TestHelper.shutdown(cluster, walletMgr, testSuccessful=testSuccessful, killEosInstances=killEosInstances, killWallet=killWallet, keepLogs=keepLogs, cleanRun=killAll, dumpErrorDetails=dumpErrorDetails)
    if not testSuccessful:
        Print(Utils.FileDivider)
        Print("Compare Blocklog")
        cluster.compareBlockLogs()
        Print(Utils.FileDivider)
        Print("Print Blocklog")
        cluster.printBlockLog()
        Print(Utils.FileDivider)

errorCode = 0 if testSuccessful else 1
exit(errorCode)
