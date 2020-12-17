/* database_connexion.cc
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

// Ourselves:
#include <snemo/services/database/database_connexion.h>

// // MySQL C++ API
// #include <mysql++/mysql++.h>

// // MariaDB C API
// #include <mariadb/mysql.h>

namespace database {

  connexion::connexion (const std::string& url      /* = "" */,
			const std::string& user     /* = "" */,
			const std::string& password /* = "" */)
    // : _database_server_ (false)
  {
    std::cout << "database::connexion(" << url.data() << ", " << user.data() << ", " << password.data() << ")" << std::endl;

    // try
    //   {
    //     // _database_server_.connect("supernemo_demo", url.data(), user.data(), password.data(), 3306);
    //   }
    // catch (const mysqlpp::BadQuery& er)
    //   {
    // 	std::cout << "DB error: " << er.what() << std::endl;
    //   }
    

    // try
    //   {
    // 	mysqlpp::Connection connection (true);
    // 	connection.connect("supernemo_demo", "ccmysql.in2p3.fr", "supernemo_lpsc", "mNUf3r6fAbCp", 3306);
    // 	std::cout << "DB connection failed: " << connection.error() << std::endl;
    //   }
    // catch (const mysqlpp::BadQuery& er)
    //   {
    // 	std::cout << "DB error: " << er.what() << std::endl;
    //   }


    // if (_database_server_.connected())
    //   {
    // 	mysqlpp::Query db_query = _database_server_.query("show databases");  
    // 	mysqlpp::StoreQueryResult db_query_results = db_query.store();
    
    // 	std::cout << db_query_results.size() << " database(s) available : " << std::endl;
    // 	mysqlpp::StoreQueryResult::const_iterator db_query_results_it;
    // 	for (db_query_results_it = db_query_results.begin(); db_query_results_it != db_query_results.end(); ++db_query_results_it)
    // 	  std::cout << '\t' << (*db_query_results_it)[0] << std::endl;
    
    // 	// show tables
    // 	mysqlpp::Query tables_query = _database_server_.query("show tables");
    // 	mysqlpp::StoreQueryResult tables_results = tables_query.store();
    // 	std::vector<std::string> tables_vector;
    // 	for (size_t t=0; t<tables_results.size(); ++t)
    // 	  {
    // 	    std::string table (tables_results[t][0]);
    // 	    tables_vector.push_back(table);
    // 	  }

    // 	// show tables info
    // 	for (size_t t=0; t<tables_vector.size(); ++t)
    // 	  {
    // 	    mysqlpp::Query query = _database_server_.query();
    // 	    query << "describe " << tables_vector[t];

    // 	    mysqlpp::StoreQueryResult result = query.store();
    // 	    size_t columns = result.num_fields();

    // 	    std::cout << tables_vector[t] << " :";
    // 	    for (size_t i = 0; i < columns; ++i)
    // 	      std::cout << " " << result.field_name(int(i));
    // 	    std::cout << std::endl;
    // 	  }
    //   }
    
    // if (mysqlpp::UseQueryResult db_query_result = db_query.use()) {
    
    // std::string product_name (_database_server_->GetDBMS());
    // std::cout << "=== product_name = " << product_name << std::endl; // "MySQL" "Oracle"

    // std::string server_info (_database_server_->ServerInfo()); // version
    // std::cout << "server_info = " << server_info << std::endl;
  
    // TSQLStatement* show_tables_statement = _database_server_->Statement("show tables");
    // if (show_tables_statement->Process()) {
    //   show_tables_statement->StoreResult();
    //   while (show_tables_statement->NextResultRow()) {
    // 	std::string table (show_tables_statement->GetString(0));
    // 	std::cout << table << std::endl;
    //   }
    // }
    // delete show_tables_statement;
  }

  connexion::~connexion() 
  {
    // if (_database_server_.connected())
    //   _database_server_.disconnect();
  }

  void connexion::connect()
  {
    // mysqlpp::Connection connection (true);
    // connection.connect("supernemo_demo", "ccmysql.in2p3.fr", "supernemo_lpsc", "mNUf3r6fAbCp", 3306);

    std::cout << "database_connexion::connect()" << std::endl;

    // // mysql_library_init(0, NULL, NULL);

    // MYSQL *mysql = NULL;
    // mysql = mysql_init(mysql);

    // if (mysql_real_connect(mysql, "ccmysql.in2p3.fr", "supernemo_lpsc", "mNUf3r6fAbCp", "supernemo_demo", 3306, NULL, CLIENT_FOUND_ROWS))
    //   std::cout << "connection to DB succeded" << std::endl;
    // else 
    //   std::cout << "connection to DB failed" << std::endl;

  }

  // void  connexion::table_exists(const std::string& tableName) 
  // {
  //   if (tableName == "") {
  //     TSQLStatement* stmt =  CreatePreparedStatement("show tables");
  //     if ( stmt ) {
  //       if (stmt->Process()) {
  //         stmt->StoreResult();
  //         while (stmt->NextResultRow()) {
  // 	  std::string tn(stmt->GetString(0));
  //           this->SetTableExists(tn);
  //         }
  //       }
  //       delete stmt;
  //       stmt = 0;
  //     }
  //   }
  //   else {
  //     if ( ! this->TableExists(tableName) ) {
  //       fExistingTableList += ",'";
  //       fExistingTableList += tableName;
  //       fExistingTableList += "'";
  //     }
  //   }
  // }

  //.....................................................................

  // Bool_t  DbiConnection::TableExists(const std::string& tableName) const 
  // {
  //   std::string test("'");
  //   test += tableName;
  //   test += "'";
  //   return fExistingTableList.find(test) != std::string::npos;
  // }

} // end of namespace database
