#include <boost/test/unit_test.hpp>
#include <eosio/chain/contract_table_objects.hpp>
#include <eosio/chain/global_property_object.hpp>
#include <eosio/chain/resource_limits.hpp>
#include <eosio/chain/wast_to_wasm.hpp>

#include <Runtime/Runtime.h>

#include "eosio_system_tester.hpp"

using namespace eosio_system;

BOOST_AUTO_TEST_SUITE(eosio_system_tests)


BOOST_FIXTURE_TEST_CASE( stake_unstake_gnode, eosio_system_tester ) try {

   produce_blocks( 1 );

   BOOST_REQUIRE_EQUAL( core_from_string("0.0000"), get_balance( "alice1111111" ) );
   transfer( "eonio", "alice1111111", core_from_string("0.1000"), "eonio" );
   produce_blocks( 1 );
   
   BOOST_REQUIRE_EQUAL( core_from_string("0.1000"), get_balance( "alice1111111" ) );

   BOOST_REQUIRE_EQUAL( wasm_assert_msg("overdrawn balance"), stake_to_gnode_t("alice1111111") ); //最少要1.0000 EON


   transfer( "eonio", "alice1111111", core_from_string("0.9000"), "eonio" );
   produce_blocks( 1 );
   
   BOOST_REQUIRE_EQUAL( core_from_string("1.0000"), get_balance( "alice1111111" ) );

   BOOST_REQUIRE_EQUAL( success(), stake_to_gnode_t("alice1111111") );

   BOOST_REQUIRE_EQUAL( core_from_string("0.0000"), get_balance( "alice1111111" ) ); //成为gnode花费1.0000 EON

   BOOST_REQUIRE_EQUAL( wasm_assert_msg("account already in _gnode"), stake_to_gnode_t("alice1111111") ); //不能重复成为gnode

   BOOST_REQUIRE_EQUAL( wasm_assert_msg("account not in _gnode"), unstake_gnode_t("bob111111111") ); //不是gnode，不能unstakegnode

   BOOST_REQUIRE_EQUAL( success(), unstake_gnode_t("alice1111111") );
   BOOST_REQUIRE_EQUAL( core_from_string("1.0000"), get_balance( "alice1111111" ) ); //抵押费用退回

} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE( stake_update_gnode, eosio_system_tester ) try {

   produce_blocks( 1 );

   BOOST_REQUIRE_EQUAL( core_from_string("0.0000"), get_balance( "alice1111111" ) );
   transfer( "eonio", "alice1111111", core_from_string("100.0000"), "eonio" );
   produce_blocks( 1 );
   
   BOOST_REQUIRE_EQUAL( success(), stake_to_gnode_t("alice1111111") ); 

   auto gnode_info = get_gnode_info("alice1111111");

   BOOST_REQUIRE_EQUAL("alice1111111", gnode_info["owner"]);
   BOOST_REQUIRE_EQUAL("alice1111111", gnode_info["payer"]);
   BOOST_REQUIRE_EQUAL(1, gnode_info["location"].as_uint64());
   BOOST_REQUIRE_EQUAL("dimension.com", gnode_info["url"]);

   BOOST_REQUIRE_EQUAL( success(), update_gnode_t("alice1111111", "new.com", 2) ); 

   auto new_gnode_info = get_gnode_info("alice1111111");

   BOOST_REQUIRE_EQUAL("alice1111111", new_gnode_info["owner"]);
   BOOST_REQUIRE_EQUAL("alice1111111", new_gnode_info["payer"]);
   BOOST_REQUIRE_EQUAL(2, new_gnode_info["location"].as_uint64());
   BOOST_REQUIRE_EQUAL("new.com", new_gnode_info["url"]);


} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE( new_proposals, eosio_system_tester ) try {

   produce_blocks( 1 );

   BOOST_REQUIRE_EQUAL( core_from_string("0.0000"), get_balance( "alice1111111" ) );
   transfer( "eonio", "alice1111111", core_from_string("1.1000"), "eonio" );
   produce_blocks( 1 );
   
   BOOST_REQUIRE_EQUAL( core_from_string("1.1000"), get_balance( "alice1111111" ) );

   BOOST_REQUIRE_EQUAL( wasm_assert_msg("only governance node can new proposal"), new_proposal_t("alice1111111", "alice1111111", 0, 1, 1) ); //成为gnode才可以发起提案

   BOOST_REQUIRE_EQUAL( success(), stake_to_gnode_t("alice1111111") ); //成为gnode消耗1.000 EON
   BOOST_REQUIRE_EQUAL( core_from_string("0.1000"), get_balance( "alice1111111" ) ); 

   BOOST_REQUIRE_EQUAL( wasm_assert_msg("overdrawn balance"), new_proposal_t("alice1111111", "alice1111111", 0, 1, 1) ); //创建提案最少要1.5000 EON

   transfer( "eonio", "alice1111111", core_from_string("1.5000"), "eonio" );
   BOOST_REQUIRE_EQUAL( success(), new_proposal_t("alice1111111", "alice1111111", 0, 1, 1) ); 
   BOOST_REQUIRE_EQUAL( core_from_string("0.1000"), get_balance( "alice1111111" ) ); 

} FC_LOG_AND_RETHROW()



BOOST_FIXTURE_TEST_CASE( new_proposals_type_test, eosio_system_tester ) try {

   produce_blocks( 1 );

   BOOST_REQUIRE_EQUAL( core_from_string("0.0000"), get_balance( "alice1111111" ) );
   transfer( "eonio", "alice1111111", core_from_string("1000.0000"), "eonio" );
   produce_blocks( 1 );

   BOOST_REQUIRE_EQUAL( success(), stake_to_gnode_t("alice1111111") ); // 成为gnode消耗1.000 EON

   BOOST_REQUIRE_EQUAL( wasm_assert_msg("can not add other account to bp"), new_proposal_t("alice1111111", "bob111111111", 0, 1, 1) ); 

   BOOST_REQUIRE_EQUAL( success(), new_proposal_t("alice1111111", "alice1111111", 0, 1, 1) ); 

   BOOST_REQUIRE_EQUAL( success(), new_proposal_t("alice1111111", "bob111111111", 0, 3, 0) ); 
   BOOST_REQUIRE_EQUAL( success(), new_proposal_t("alice1111111", "bob111111111", 0, 3, 1) ); 
   BOOST_REQUIRE_EQUAL( success(), new_proposal_t("alice1111111", "bob111111111", 0, 3, 2) ); 
   BOOST_REQUIRE_EQUAL( wasm_assert_msg("consensus_type must be one of 0, 1, 2"), new_proposal_t("alice1111111", "bob111111111", 0, 3, 3) ); 
   BOOST_REQUIRE_EQUAL( wasm_assert_msg("consensus_type must be one of 0, 1, 2"), new_proposal_t("alice1111111", "bob111111111", 0, 3, 4) ); 

   auto proposal = get_proposal_info(0);

   BOOST_REQUIRE_EQUAL(0, proposal["id"].as_uint64());
   BOOST_REQUIRE_EQUAL("alice1111111", proposal["owner"]);
   BOOST_REQUIRE_EQUAL("alice1111111", proposal["account"]);
   BOOST_REQUIRE_EQUAL(1, proposal["type"].as_int64());
   BOOST_REQUIRE_EQUAL(1, proposal["consensus_type"].as_int64());
   BOOST_REQUIRE_EQUAL(0, proposal["total_yeas"].as_int64());
   BOOST_REQUIRE_EQUAL(0, proposal["total_nays"].as_int64());
   BOOST_REQUIRE_EQUAL(0, proposal["total_staked"].as_int64());


} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( vote_proposals, eosio_system_tester ) try {

   produce_blocks( 1 );

   BOOST_REQUIRE_EQUAL( core_from_string("0.0000"), get_balance( "alice1111111" ) );
   transfer( "eonio", "alice1111111", core_from_string("1000.0000"), "eonio" );
   produce_blocks( 1 );

   BOOST_REQUIRE_EQUAL( success(), stake_to_gnode_t("alice1111111") ); // 成为gnode消耗1.000 EON


   BOOST_REQUIRE_EQUAL( success(), new_proposal_t("alice1111111", "alice1111111", 0, 1, 1) ); 

   auto proposal = get_proposal_info(0);
   BOOST_REQUIRE_EQUAL(0, proposal["total_yeas"].as_int64());
   BOOST_REQUIRE_EQUAL(0, proposal["total_nays"].as_int64());
   BOOST_REQUIRE_EQUAL(0, proposal["total_staked"].as_int64());

   stake( "eonio", "bob111111111", core_from_string("1000.0000"), core_from_string("0.0000") );

   BOOST_REQUIRE_EQUAL( wasm_assert_msg("proposal not exist"), vote_proposal_t("bob111111111", 1, true) ); //对id为1的投票 
   BOOST_REQUIRE_EQUAL( success(), vote_proposal_t("bob111111111", 0, true) ); //对id为0的投票 


   proposal = get_proposal_info(0);
   BOOST_REQUIRE_EQUAL(1, proposal["type"].as_int64());
   BOOST_REQUIRE_EQUAL(1, proposal["consensus_type"].as_int64());
//    BOOST_REQUIRE_EQUAL(11, proposal["total_yeas"].as_int64());
   BOOST_REQUIRE_EQUAL(0, proposal["total_nays"].as_int64());
   BOOST_REQUIRE_EQUAL(0, proposal["total_staked"].as_int64());

   BOOST_REQUIRE_EQUAL( success(), vote_proposal_t("bob111111111", 0, false) ); //对id为0的投反对票 

   proposal = get_proposal_info(0);
   BOOST_REQUIRE_EQUAL(0, proposal["total_yeas"].as_int64());
//    BOOST_REQUIRE_EQUAL(11, proposal["total_nays"].as_int64());
   BOOST_REQUIRE_EQUAL(0, proposal["total_staked"].as_int64());

   produce_block(fc::minutes(2*60-1)); //接近2小时
   BOOST_REQUIRE_EQUAL( success(), vote_proposal_t("bob111111111", 0, true) );
   produce_block(fc::minutes(1));
   produce_blocks(1);

   BOOST_REQUIRE_EQUAL( wasm_assert_msg("proposal vote time has elapsed"), vote_proposal_t("bob111111111", 0, true) ); //超时

} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( exec_proposals, eosio_system_tester ) try {

   produce_blocks( 1 );

   BOOST_REQUIRE_EQUAL( core_from_string("0.0000"), get_balance( "alice1111111" ) );
   transfer( "eonio", "alice1111111", core_from_string("1000.0000"), "eonio" );
   produce_blocks( 1 );

   BOOST_REQUIRE_EQUAL( success(), stake_to_gnode_t("alice1111111") ); // 成为gnode消耗1.000 EON
   BOOST_REQUIRE_EQUAL( success(), new_proposal_t("alice1111111", "alice1111111", 0, 1, 1) ); 
   BOOST_REQUIRE_EQUAL( success(), new_proposal_t("alice1111111", "alice1111111", 0, 3, 1) ); 

   stake( "eonio", "bob111111111", core_from_string("100000000.0000"), core_from_string("0.0000") );
   BOOST_REQUIRE_EQUAL( success(), vote_proposal_t("bob111111111", 0, true) ); //对id为0的投票 

   produce_block(fc::hours(2)); //2小时
   produce_blocks(1);

   BOOST_REQUIRE_EQUAL( wasm_assert_msg("only governance node can exec proposal"), exec_proposal_t("bob111111111", 0) ); 
   BOOST_REQUIRE_EQUAL( wasm_assert_msg("proposal_id not in _proposals"), exec_proposal_t("alice1111111", 2) ); //id为2的不存在
   BOOST_REQUIRE_EQUAL( success(), exec_proposal_t("alice1111111", 0) );
   BOOST_REQUIRE_EQUAL( wasm_assert_msg("proposal has been executed"), exec_proposal_t("alice1111111", 0) );  //重复执行id为0的提案

   produce_block(fc::hours(24 * 3)); //3天后
   BOOST_REQUIRE_EQUAL( wasm_assert_msg("proposal execution time has elapsed"), exec_proposal_t("alice1111111", 1) );  //过了执行提案时间

} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE( add_producers, eosio_system_tester ) try {

   produce_blocks( 1 );

   
} FC_LOG_AND_RETHROW()



BOOST_FIXTURE_TEST_CASE( stake_negative, eosio_system_tester ) try {
   issue( "alice1111111", core_from_string("1000.0000"),  config::system_account_name );

   BOOST_REQUIRE_EQUAL( wasm_assert_msg("must stake a positive amount"),
                        stake( "alice1111111", core_from_string("-0.0001"), core_from_string("0.0000") )
   );

   BOOST_REQUIRE_EQUAL( wasm_assert_msg("must stake a positive amount"),
                        stake( "alice1111111", core_from_string("0.0000"), core_from_string("-0.0001") )
   );

   BOOST_REQUIRE_EQUAL( wasm_assert_msg("must stake a positive amount"),
                        stake( "alice1111111", core_from_string("00.0000"), core_from_string("00.0000") )
   );

   BOOST_REQUIRE_EQUAL( wasm_assert_msg("must stake a positive amount"),
                        stake( "alice1111111", core_from_string("0.0000"), core_from_string("00.0000") )

   );

   BOOST_REQUIRE_EQUAL( true, get_voter_info( "alice1111111" ).is_null() );
} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( unstake_negative, eosio_system_tester ) try {
   issue( "alice1111111", core_from_string("1000.0000"),  config::system_account_name );

   BOOST_REQUIRE_EQUAL( success(), stake( "alice1111111", "bob111111111", core_from_string("200.0001"), core_from_string("100.0001") ) );

   auto total = get_total_stake( "bob111111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("210.0001"), total["net_weight"].as<asset>());
   auto vinfo = get_voter_info("alice1111111" );
   wdump((vinfo));
   REQUIRE_MATCHING_OBJECT( voter( "alice1111111", core_from_string("300.0002") ), get_voter_info( "alice1111111" ) );


   BOOST_REQUIRE_EQUAL( wasm_assert_msg("must unstake a positive amount"),
                        unstake( "alice1111111", "bob111111111", core_from_string("-1.0000"), core_from_string("0.0000") )
   );

   BOOST_REQUIRE_EQUAL( wasm_assert_msg("must unstake a positive amount"),
                        unstake( "alice1111111", "bob111111111", core_from_string("0.0000"), core_from_string("-1.0000") )
   );

   //unstake all zeros
   BOOST_REQUIRE_EQUAL( wasm_assert_msg("must unstake a positive amount"),
                        unstake( "alice1111111", "bob111111111", core_from_string("0.0000"), core_from_string("0.0000") )

   );

} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( unstake_more_than_at_stake, eosio_system_tester ) try {
   cross_15_percent_threshold();

   issue( "alice1111111", core_from_string("1000.0000"),  config::system_account_name );
   BOOST_REQUIRE_EQUAL( success(), stake( "alice1111111", core_from_string("200.0000"), core_from_string("100.0000") ) );

   auto total = get_total_stake( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("210.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("110.0000"), total["cpu_weight"].as<asset>());

   BOOST_REQUIRE_EQUAL( core_from_string("700.0000"), get_balance( "alice1111111" ) );

   //trying to unstake more net bandwith than at stake

   BOOST_REQUIRE_EQUAL( wasm_assert_msg("insufficient staked net bandwidth"),
                        unstake( "alice1111111", core_from_string("200.0001"), core_from_string("0.0000") )
   );

   //trying to unstake more cpu bandwith than at stake
   BOOST_REQUIRE_EQUAL( wasm_assert_msg("insufficient staked cpu bandwidth"),
                        unstake( "alice1111111", core_from_string("0.0000"), core_from_string("100.0001") )

   );

   //check that nothing has changed
   total = get_total_stake( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("210.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("110.0000"), total["cpu_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("700.0000"), get_balance( "alice1111111" ) );
} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( delegate_to_another_user, eosio_system_tester ) try {
   cross_15_percent_threshold();

   issue( "alice1111111", core_from_string("1000.0000"),  config::system_account_name );

   BOOST_REQUIRE_EQUAL( success(), stake ( "alice1111111", "bob111111111", core_from_string("200.0000"), core_from_string("100.0000") ) );

   auto total = get_total_stake( "bob111111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("210.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("110.0000"), total["cpu_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("700.0000"), get_balance( "alice1111111" ) );
   //all voting power goes to alice1111111
   REQUIRE_MATCHING_OBJECT( voter( "alice1111111", core_from_string("300.0000") ), get_voter_info( "alice1111111" ) );
   //but not to bob111111111
   BOOST_REQUIRE_EQUAL( true, get_voter_info( "bob111111111" ).is_null() );

   //bob111111111 should not be able to unstake what was staked by alice1111111
   BOOST_REQUIRE_EQUAL( wasm_assert_msg("insufficient staked cpu bandwidth"),
                        unstake( "bob111111111", core_from_string("0.0000"), core_from_string("10.0000") )

   );
   BOOST_REQUIRE_EQUAL( wasm_assert_msg("insufficient staked net bandwidth"),
                        unstake( "bob111111111", core_from_string("10.0000"),  core_from_string("0.0000") )
   );

   issue( "carol1111111", core_from_string("1000.0000"),  config::system_account_name );
   BOOST_REQUIRE_EQUAL( success(), stake( "carol1111111", "bob111111111", core_from_string("20.0000"), core_from_string("10.0000") ) );
   total = get_total_stake( "bob111111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("230.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("120.0000"), total["cpu_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("970.0000"), get_balance( "carol1111111" ) );
   REQUIRE_MATCHING_OBJECT( voter( "carol1111111", core_from_string("30.0000") ), get_voter_info( "carol1111111" ) );

   //alice1111111 should not be able to unstake money staked by carol1111111

   BOOST_REQUIRE_EQUAL( wasm_assert_msg("insufficient staked net bandwidth"),
                        unstake( "alice1111111", "bob111111111", core_from_string("2001.0000"), core_from_string("1.0000") )
   );

   BOOST_REQUIRE_EQUAL( wasm_assert_msg("insufficient staked cpu bandwidth"),
                        unstake( "alice1111111", "bob111111111", core_from_string("1.0000"), core_from_string("101.0000") )

   );

   total = get_total_stake( "bob111111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("230.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("120.0000"), total["cpu_weight"].as<asset>());
   //balance should not change after unsuccessfull attempts to unstake
   BOOST_REQUIRE_EQUAL( core_from_string("700.0000"), get_balance( "alice1111111" ) );
   //voting power too
   REQUIRE_MATCHING_OBJECT( voter( "alice1111111", core_from_string("300.0000") ), get_voter_info( "alice1111111" ) );
   REQUIRE_MATCHING_OBJECT( voter( "carol1111111", core_from_string("30.0000") ), get_voter_info( "carol1111111" ) );
   BOOST_REQUIRE_EQUAL( true, get_voter_info( "bob111111111" ).is_null() );
} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( stake_unstake_separate, eosio_system_tester ) try {
   cross_15_percent_threshold();

   issue( "alice1111111", core_from_string("1000.0000"),  config::system_account_name );
   BOOST_REQUIRE_EQUAL( core_from_string("1000.0000"), get_balance( "alice1111111" ) );

   //everything at once
   BOOST_REQUIRE_EQUAL( success(), stake( "alice1111111", core_from_string("10.0000"), core_from_string("20.0000") ) );
   auto total = get_total_stake( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("20.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("30.0000"), total["cpu_weight"].as<asset>());

   //cpu
   BOOST_REQUIRE_EQUAL( success(), stake( "alice1111111", core_from_string("100.0000"), core_from_string("0.0000") ) );
   total = get_total_stake( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("120.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("30.0000"), total["cpu_weight"].as<asset>());

   //net
   BOOST_REQUIRE_EQUAL( success(), stake( "alice1111111", core_from_string("0.0000"), core_from_string("200.0000") ) );
   total = get_total_stake( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("120.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("230.0000"), total["cpu_weight"].as<asset>());

   //unstake cpu
   BOOST_REQUIRE_EQUAL( success(), unstake( "alice1111111", core_from_string("100.0000"), core_from_string("0.0000") ) );
   total = get_total_stake( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("20.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("230.0000"), total["cpu_weight"].as<asset>());

   //unstake net
   BOOST_REQUIRE_EQUAL( success(), unstake( "alice1111111", core_from_string("0.0000"), core_from_string("200.0000") ) );
   total = get_total_stake( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("20.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("30.0000"), total["cpu_weight"].as<asset>());
} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( stake_from_refund, eosio_system_tester ) try {
   cross_15_percent_threshold();

   issue( "alice1111111", core_from_string("1000.0000"),  config::system_account_name );
   BOOST_REQUIRE_EQUAL( success(), stake( "alice1111111", "alice1111111", core_from_string("200.0000"), core_from_string("100.0000") ) );

   auto total = get_total_stake( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("210.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("110.0000"), total["cpu_weight"].as<asset>());

   BOOST_REQUIRE_EQUAL( success(), stake( "alice1111111", "bob111111111", core_from_string("50.0000"), core_from_string("50.0000") ) );

   total = get_total_stake( "bob111111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("60.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("60.0000"), total["cpu_weight"].as<asset>());

   REQUIRE_MATCHING_OBJECT( voter( "alice1111111", core_from_string("400.0000") ), get_voter_info( "alice1111111" ) );
   BOOST_REQUIRE_EQUAL( core_from_string("600.0000"), get_balance( "alice1111111" ) );

   //unstake a share
   BOOST_REQUIRE_EQUAL( success(), unstake( "alice1111111", "alice1111111", core_from_string("100.0000"), core_from_string("50.0000") ) );
   total = get_total_stake( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("110.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("60.0000"), total["cpu_weight"].as<asset>());
   REQUIRE_MATCHING_OBJECT( voter( "alice1111111", core_from_string("250.0000") ), get_voter_info( "alice1111111" ) );
   BOOST_REQUIRE_EQUAL( core_from_string("600.0000"), get_balance( "alice1111111" ) );
   auto refund = get_refund_request( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("100.0000"), refund["net_amount"].as<asset>() );
   BOOST_REQUIRE_EQUAL( core_from_string( "50.0000"), refund["cpu_amount"].as<asset>() );
   //XXX auto request_time = refund["request_time"].as_int64();

   //alice delegates to bob, should pull from liquid balance not refund
   BOOST_REQUIRE_EQUAL( success(), stake( "alice1111111", "bob111111111", core_from_string("50.0000"), core_from_string("50.0000") ) );
   total = get_total_stake( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("110.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("60.0000"), total["cpu_weight"].as<asset>());
   REQUIRE_MATCHING_OBJECT( voter( "alice1111111", core_from_string("350.0000") ), get_voter_info( "alice1111111" ) );
   BOOST_REQUIRE_EQUAL( core_from_string("500.0000"), get_balance( "alice1111111" ) );
   refund = get_refund_request( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("100.0000"), refund["net_amount"].as<asset>() );
   BOOST_REQUIRE_EQUAL( core_from_string( "50.0000"), refund["cpu_amount"].as<asset>() );

   //stake less than pending refund, entire amount should be taken from refund
   BOOST_REQUIRE_EQUAL( success(), stake( "alice1111111", "alice1111111", core_from_string("50.0000"), core_from_string("25.0000") ) );
   total = get_total_stake( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("160.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("85.0000"), total["cpu_weight"].as<asset>());
   refund = get_refund_request( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("50.0000"), refund["net_amount"].as<asset>() );
   BOOST_REQUIRE_EQUAL( core_from_string("25.0000"), refund["cpu_amount"].as<asset>() );
   //request time should stay the same
   //BOOST_REQUIRE_EQUAL( request_time, refund["request_time"].as_int64() );
   //balance should stay the same
   BOOST_REQUIRE_EQUAL( core_from_string("500.0000"), get_balance( "alice1111111" ) );

   //stake exactly pending refund amount
   BOOST_REQUIRE_EQUAL( success(), stake( "alice1111111", "alice1111111", core_from_string("50.0000"), core_from_string("25.0000") ) );
   total = get_total_stake( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("210.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("110.0000"), total["cpu_weight"].as<asset>());
   //pending refund should be removed
   refund = get_refund_request( "alice1111111" );
   BOOST_TEST_REQUIRE( refund.is_null() );
   //balance should stay the same
   BOOST_REQUIRE_EQUAL( core_from_string("500.0000"), get_balance( "alice1111111" ) );

   //create pending refund again
   BOOST_REQUIRE_EQUAL( success(), unstake( "alice1111111", "alice1111111", core_from_string("200.0000"), core_from_string("100.0000") ) );
   total = get_total_stake( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("10.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("10.0000"), total["cpu_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("500.0000"), get_balance( "alice1111111" ) );
   refund = get_refund_request( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("200.0000"), refund["net_amount"].as<asset>() );
   BOOST_REQUIRE_EQUAL( core_from_string("100.0000"), refund["cpu_amount"].as<asset>() );

   //stake more than pending refund
   BOOST_REQUIRE_EQUAL( success(), stake( "alice1111111", "alice1111111", core_from_string("300.0000"), core_from_string("200.0000") ) );
   total = get_total_stake( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("310.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("210.0000"), total["cpu_weight"].as<asset>());
   REQUIRE_MATCHING_OBJECT( voter( "alice1111111", core_from_string("700.0000") ), get_voter_info( "alice1111111" ) );
   refund = get_refund_request( "alice1111111" );
   BOOST_TEST_REQUIRE( refund.is_null() );
   //200 core tokens should be taken from alice's account
   BOOST_REQUIRE_EQUAL( core_from_string("300.0000"), get_balance( "alice1111111" ) );

} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE( stake_to_another_user_not_from_refund, eosio_system_tester ) try {
   cross_15_percent_threshold();

   issue( "alice1111111", core_from_string("1000.0000"),  config::system_account_name );
   BOOST_REQUIRE_EQUAL( success(), stake( "alice1111111", core_from_string("200.0000"), core_from_string("100.0000") ) );

   auto total = get_total_stake( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("210.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("110.0000"), total["cpu_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("700.0000"), get_balance( "alice1111111" ) );

   REQUIRE_MATCHING_OBJECT( voter( "alice1111111", core_from_string("300.0000") ), get_voter_info( "alice1111111" ) );
   BOOST_REQUIRE_EQUAL( core_from_string("700.0000"), get_balance( "alice1111111" ) );

   //unstake
   BOOST_REQUIRE_EQUAL( success(), unstake( "alice1111111", core_from_string("200.0000"), core_from_string("100.0000") ) );
   auto refund = get_refund_request( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("200.0000"), refund["net_amount"].as<asset>() );
   BOOST_REQUIRE_EQUAL( core_from_string("100.0000"), refund["cpu_amount"].as<asset>() );
   //auto orig_request_time = refund["request_time"].as_int64();

   //stake to another user
   BOOST_REQUIRE_EQUAL( success(), stake( "alice1111111", "bob111111111", core_from_string("200.0000"), core_from_string("100.0000") ) );
   total = get_total_stake( "bob111111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("210.0000"), total["net_weight"].as<asset>());
   BOOST_REQUIRE_EQUAL( core_from_string("110.0000"), total["cpu_weight"].as<asset>());
   //stake should be taken from alices' balance, and refund request should stay the same
   BOOST_REQUIRE_EQUAL( core_from_string("400.0000"), get_balance( "alice1111111" ) );
   refund = get_refund_request( "alice1111111" );
   BOOST_REQUIRE_EQUAL( core_from_string("200.0000"), refund["net_amount"].as<asset>() );
   BOOST_REQUIRE_EQUAL( core_from_string("100.0000"), refund["cpu_amount"].as<asset>() );
   //BOOST_REQUIRE_EQUAL( orig_request_time, refund["request_time"].as_int64() );

} FC_LOG_AND_RETHROW()





fc::mutable_variant_object config_to_variant( const eosio::chain::chain_config& config ) {
   return mutable_variant_object()
      ( "max_block_net_usage", config.max_block_net_usage )
      ( "target_block_net_usage_pct", config.target_block_net_usage_pct )
      ( "max_transaction_net_usage", config.max_transaction_net_usage )
      ( "base_per_transaction_net_usage", config.base_per_transaction_net_usage )
      ( "context_free_discount_net_usage_num", config.context_free_discount_net_usage_num )
      ( "context_free_discount_net_usage_den", config.context_free_discount_net_usage_den )
      ( "max_block_cpu_usage", config.max_block_cpu_usage )
      ( "target_block_cpu_usage_pct", config.target_block_cpu_usage_pct )
      ( "max_transaction_cpu_usage", config.max_transaction_cpu_usage )
      ( "min_transaction_cpu_usage", config.min_transaction_cpu_usage )
      ( "max_transaction_lifetime", config.max_transaction_lifetime )
      ( "deferred_trx_expiration_window", config.deferred_trx_expiration_window )
      ( "max_transaction_delay", config.max_transaction_delay )
      ( "max_inline_action_size", config.max_inline_action_size )
      ( "max_inline_action_depth", config.max_inline_action_depth )
      ( "max_authority_depth", config.max_authority_depth );
}



BOOST_AUTO_TEST_SUITE_END()
