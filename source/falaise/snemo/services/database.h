//! \file falaise/snemo/services/database.h
//! \brief Types and traits for the SuperNEMO Database Service
#ifndef SNEMO_DATABASE_SVC_HH
#define SNEMO_DATABASE_SVC_HH

#include "falaise/snemo/services/database/database_service.h"
#include "falaise/snemo/services/database/manager.h"

#include "falaise/snemo/services/service_traits.h"

namespace snemo {
/*! \class snemo::database_svc
 *  \brief SuperNEMO detector database service
 *
 * It should/will provide implementation of database access
 * such as connection and read/write operation
 */
using database_svc = const database::manager;

//! Specialization of service_traits for database_svc
template <>
struct service_traits<database_svc> {
  using label_type = BOOST_METAPARSE_STRING("database");
  using service_type = database::database_service;
  using instance_type = database_svc;

  static instance_type* get(service_type& sm) {
    return &(sm.get_db_manager());
  }
};

} // namespace snemo

#endif // SNEMO_DATABASE_SVC_HH

