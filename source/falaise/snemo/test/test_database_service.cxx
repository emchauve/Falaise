// Catch
#include "catch.hpp"

#include "falaise/snemo/services/database.h"
#include "bayeux/datatools/service_manager.h"

TEST_CASE("Default construction works", "") {
  snemo::service_handle<snemo::database_svc> x;
  REQUIRE_THROWS_AS(x->is_debug(), snemo::bad_service_access);
}
