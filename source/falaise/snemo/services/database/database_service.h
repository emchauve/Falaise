/// \file falaise/snemo/services/database_service.h
/* Author(s)     :     Emmanuel Chauveau <chauveau@cenbg.in2p3.fr>
 * Creation date : 2020-01-17
 * Last modified : 2020-01-17
 *
 * Copyright (C) 2011-2013 Francois Mauger <mauger@lpccaen.in2p3.fr>
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
 * Description:
 *
 *   Generic database service.
 *
 * History:
 *
 */

#ifndef DATABASE_SERVICE_H
#define DATABASE_SERVICE_H 1

// Standard library:
#include <iostream>
#include <string>

// Third party :
// - Bayeux/datatools:
#include <datatools/base_service.h>
#include <datatools/service_tools.h>

namespace datatools {
  class properties;
}

namespace database {

  class manager;

  /// \brief Generic database service
  class database_service : public datatools::base_service
  {

  public:

    /// Check initialization flag
    virtual bool is_initialized () const;

    /// Initialization
    virtual int initialize (const datatools::properties & a_config,
                            datatools::service_dict_type & a_service_dict);

    /// Termination
    virtual int reset ();

    /// Return a reference to the non mutable database manager
    const database::manager & get_db_manager () const;

    /// Default constructor
    database_service ();

    /// Destructor
    virtual ~database_service ();

    /// Smart print
    virtual void tree_dump (std::ostream & a_out         = std::clog,
                            const std::string & a_title  = "",
                            const std::string & a_indent = "",
                            bool a_inherit               = false) const;

  private:

    bool                 _initialized_; //!< Initialization flag
    database::manager * _db_manager_; //!< Handle to the database manager

    // Registration :
    DATATOOLS_SERVICE_REGISTRATION_INTERFACE(database_service)

  };

}  // end of namespace database

#include <datatools/ocd_macros.h>

// Declare the OCD interface of the module
DOCD_CLASS_DECLARATION(database::database_service)

#endif // DATABASE_SERVICE_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
