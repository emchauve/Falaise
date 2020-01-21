// Catch
#include "catch.hpp"

#include "bayeux/datatools/service_manager.h"
#include "falaise/snemo/services/service_handle.h"
#include "falaise/snemo/services/database.h"

TEST_CASE("Default construction works", "") {
  snemo::service_handle<snemo::database_svc> x;
  REQUIRE_THROWS_AS(x->is_debug(), snemo::bad_service_access);
}

TEST_CASE("Construction from good services works", "") {
  datatools::service_manager dummyServices{};
  datatools::multi_properties config;
  config.add_section("database", "database::database_service")
      .store_path("manager.configuration_file", "@falaise:snemo/demonstrator/database/DatabaseManager.conf");

  dummyServices.load(config);
  dummyServices.initialize();

  SECTION("manager-holder service works") {
    snemo::service_handle<snemo::database_svc> x{dummyServices};
    REQUIRE_NOTHROW(x->is_debug());
  }
}
