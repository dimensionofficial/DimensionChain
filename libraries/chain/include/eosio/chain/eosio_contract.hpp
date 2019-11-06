/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#pragma once

#include <eosio/chain/types.hpp>
#include <eosio/chain/contract_types.hpp>

namespace eosio { namespace chain {

   class apply_context;

   /**
    * @defgroup native_action_handlers Native Action Handlers
    */
   ///@{
   void apply_eonio_newaccount(apply_context&);
   void apply_eonio_updateauth(apply_context&);
   void apply_eonio_deleteauth(apply_context&);
   void apply_eonio_linkauth(apply_context&);
   void apply_eonio_unlinkauth(apply_context&);

   /*
   void apply_eonio_postrecovery(apply_context&);
   void apply_eonio_passrecovery(apply_context&);
   void apply_eonio_vetorecovery(apply_context&);
   */

   void apply_eonio_setcode(apply_context&);
   void apply_eonio_setabi(apply_context&);
   
   void apply_eonio_canceldelay(apply_context&);
   ///@}  end action handlers

} } /// namespace eosio::chain
