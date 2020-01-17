/* manager.cc
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
 * You should have received a copy of the GNU General Public  License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

// Ourselves:
#include <snemo/services/database/manager.h>

// Standard library:
#include <cstdlib>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <list>
#include <map>

// Third party:
// - Bayeux/datatools:
#include <datatools/utils.h>
#include <datatools/ioutils.h>
#include <datatools/units.h>
#include <datatools/version_id.h>
#include <datatools/version_check.h>
#include <datatools/exception.h>

namespace database {

  DATATOOLS_FACTORY_SYSTEM_REGISTER_IMPLEMENTATION(manager::base_plugin,
                                                   "database::manager::base_plugin/__system__")

  datatools::logger::priority manager::base_plugin::get_logging_priority() const
  {
    return _logging;
  }

  void manager::base_plugin::set_logging_priority(datatools::logger::priority lp_)
  {
    _logging = lp_;
    return;
  }

  manager::base_plugin::base_plugin()
  {
    _logging = datatools::logger::PRIO_WARNING;
    _db_mgr_ = 0;
    return;
  }

  manager::base_plugin::~base_plugin()
  {
    _db_mgr_ = 0;
    _db_setup_requirements_.clear();
    return;
  }

  void manager::base_plugin::set_db_manager(manager & db_mgr_)
  {
    DT_THROW_IF (is_initialized(),
                 std::logic_error,
                 "Cannot set the database manager ! Plugin is already initialized !");
    _db_mgr_ = &db_mgr_;
    return;
  }

  const manager & manager::base_plugin::get_db_manager() const
  {
    return *_db_mgr_;
  }

  manager & manager::base_plugin::_grab_db_manager()
  {
    return *_db_mgr_;
  }

  bool manager::base_plugin::_has_db_setup_requirement() const
  {
    return ! _db_setup_requirements_.empty();
  }

  void manager::base_plugin::_check_db_setup_requirement() const
  {
    DT_THROW_IF(_db_mgr_ == 0, std::logic_error,
                "The database manager has not been set !");
    if (_has_db_setup_requirement()) {
      const std::string & db_mgr_setup_label = _db_mgr_->get_setup_label ();
      const std::string & db_mgr_setup_version = _db_mgr_->get_setup_version ();
      datatools::version_id db_mgr_setup_vid;
      {
        std::istringstream iss (db_mgr_setup_version);
        iss >> db_mgr_setup_vid;
      }
      for (size_t i = 0; i < _db_setup_requirements_.size(); i++)
        {
          if (datatools::validate_version (db_mgr_setup_label,
                                           db_mgr_setup_vid,
                                           _db_setup_requirements_[i]))
            {
              DT_LOG_DEBUG(get_logging_priority(), "Database setup is validated");
              return;
            }
        }
      DT_THROW_IF(true, std::logic_error,
                  "Database manager setup label '" << _db_mgr_->get_setup_label ()
                  << "' with version '" << db_mgr_setup_vid
                  << "' does not match one of the requested setup requirement");
    }
  }

  int manager::base_plugin::initialize_standalone(const datatools::properties& config_)
  {
    plugins_dict_type dummy;
    return initialize_simple(config_, dummy);
  }

  int manager::base_plugin::initialize_simple(const datatools::properties & config_,
                                              const plugins_dict_type & plugins_)
  {
    datatools::service_dict_type dummy;
    return initialize(config_, plugins_, dummy);
  }

  void manager::base_plugin::_basic_initialize(const datatools::properties & config_)
  {
    set_logging_priority(datatools::logger::extract_logging_configuration(config_,datatools::logger::PRIO_WARNING));

    if (config_.has_key("database.setup_requirement"))
      {
        DT_LOG_NOTICE(get_logging_priority(),
                      "Loading 'database.setup_requirement' rules...");
        config_.fetch("database.setup_requirement", _db_setup_requirements_);
        _check_db_setup_requirement();
      }
  }

  /************************************************/

  bool manager::plugin_entry::is_created() const
  {
    return _status_ & plugin_entry::STATUS_CREATED;
  }

  bool manager::plugin_entry::is_initialized() const
  {
    return _status_ & plugin_entry::STATUS_INITIALIZED;
  }

  void manager::plugin_entry::set_name(const std::string& name_)
  {
    DT_THROW_IF (is_created(),
                 std::logic_error,
                 "Database plugin '" << name_ << "' is already created !");
    DT_THROW_IF (name_.empty(),
                 std::logic_error,
                 "Database plugin cannot have an empty name !");
    _name_ = name_;
  }

  const std::string & manager::plugin_entry::get_name() const
  {
    return _name_;
  }

  const std::string & manager::plugin_entry::get_id() const
  {
    return _id_;
  }

  void manager::plugin_entry::set_description(const std::string& description_)
  {
    DT_THROW_IF (is_created(),
                 std::logic_error,
                 "Plugin is already created !");
    _description_ = description_;
    return;
  }

  bool manager::plugin_entry::has_description() const
  {
    return ! _description_.empty ();
  }

  const std::string & manager::plugin_entry::get_description() const
  {
    return _description_;
  }

  /// Set the configuration
  const datatools::properties & manager::plugin_entry::get_config () const
  {
    return _config_;
  }

  /// Get the configuration
  void manager::plugin_entry::set_config (const datatools::properties & config_)
  {
    DT_THROW_IF (is_initialized(),
                 std::logic_error,
                 "Plugin is already initialized !");
    _config_ = config_;
    return;
  }

  /// Grab the configuration
  datatools::properties & manager::plugin_entry::grab_config ()
  {
    DT_THROW_IF (is_initialized(),
                 std::logic_error,
                 "Plugin is already initialized !");
    return _config_;
  }

  void manager::plugin_entry::_set_status (uint32_t status_)
  {
    _status_ = status_;
    return;
  }

  bool manager::plugin_entry::can_be_dropped () const
  {
    return true;
  }

  manager::plugin_entry::plugin_entry()
  {
    _status_ = 0x0;
    return;
  }

  manager::plugin_entry::~plugin_entry()
  {
    _handle_.reset();
    return;
  }

  manager::base_plugin & manager::plugin_entry::grab ()
  {
    return _handle_.grab();
  }

  const manager::base_plugin & manager::plugin_entry::get () const
  {
    return _handle_.get();
  }

  void manager::plugin_entry::tree_dump(std::ostream& out_,
                                        const std::string& title_,
                                        const std::string& indent_,
                                        bool inherit_) const
  {

    std::string indent;
    if (!indent_.empty()) indent = indent_;

    if (!title_.empty()) out_ << indent << title_ << std::endl;

    out_ << indent << datatools::i_tree_dumpable::tag
        << "Plugin name     : '"
        << _name_
        << "'" << std::endl;

    out_ << indent << datatools::i_tree_dumpable::tag
        << "Plugin ID       : '"
        << _id_
        << "'" << std::endl;

    out_ << indent << datatools::i_tree_dumpable::tag
        << "Plugin description : '"
        << _description_
        << "'" << std::endl;

    out_ << indent << datatools::i_tree_dumpable::tag
        << "Can be dropped   : "
        << can_be_dropped() << std::endl;

    out_ << indent << datatools::i_tree_dumpable::inherit_tag (inherit_)
        << "Plugin status   : "
        << _status_;
    {
      size_t count = 0;
      std::ostringstream status_info;
      if (_status_ & plugin_entry::STATUS_CREATED) {
        if (count) status_info << ',';
        status_info << "created";
        count++;
      }
      if (_status_ & plugin_entry::STATUS_INITIALIZED) {
        if (count) status_info << ',';
        status_info << "initialized";
        count++;
      }
      if (count) {
        out_ << ' ' << '('
            << status_info.str()
            << ')';
      }
    }
    out_ << std::endl;
  }

  /*****************************************************/

  void manager::set_logging_priority(datatools::logger::priority logging_priority_)
  {
    _logging = logging_priority_;
    return;
  }

  datatools::logger::priority manager::get_logging_priority() const
  {
    return _logging;
  }

  bool manager::is_debug () const
  {
    return _logging >= datatools::logger::PRIO_DEBUG;
  }

  void manager::set_setup_label (const std::string & sl_)
  {
    _setup_label_ = sl_;
    return;
  }

  const std::string & manager::get_setup_label () const
  {
    return _setup_label_;
  }

  void manager::fetch_setup_version_id (datatools::version_id & vid_) const
  {
    vid_.reset ();
    if (_setup_version_.empty ()) return;
    std::istringstream iss (_setup_version_);
    iss >> vid_;
    return;
  }

  void manager::set_setup_version (const std::string & sv_)
  {
    // check the versioning format :
    if (! sv_.empty())  {
      datatools::version_id db_mgr_setup_vid;
      std::istringstream iss (sv_);
      iss >> db_mgr_setup_vid;
      DT_THROW_IF (! iss,
                   std::logic_error,
                   "Invalid version format '" << sv_ << "' !");
    }
    _setup_version_ = sv_;
    return;
  }

  const std::string & manager::get_setup_version () const
  {
    return _setup_version_;
  }

  void manager::set_setup_description (const std::string & sd_)
  {
    _setup_description_ = sd_;
    return;
  }

  const std::string & manager::get_setup_description () const
  {
    return _setup_description_;
  }

  bool manager::is_verbose () const
  {
    return _logging >= datatools::logger::PRIO_NOTICE;
  }

  bool manager::is_initialized () const
  {
    return _initialized_;
  }

  const manager::plugins_dict_type & manager::get_plugins () const
  {
    return _plugins_;
  }

  manager::plugins_dict_type & manager::grab_plugins ()
  {
    return _plugins_;
  }

  bool manager::has_plugin (const std::string & plugin_name_) const
  {
    return _plugins_.find(plugin_name_) != _plugins_.end ();
  }

  bool manager::has_plugin_type(const std::string& plugin_id_) const
  {
    return _plugins_factory_register_.has(plugin_id_);
  }

  void manager::unregister_plugin_type(const std::string& plugin_id_)
  {
    _plugins_factory_register_.unregistration(plugin_id_);
    return;
  }

  bool manager::has_services () const
  {
    return _services_;
  }

  void manager::reset_services ()
  {
    _services_ = 0;
    return;
  }

  void manager::set_services(datatools::service_dict_type & services_)
  {
    DT_THROW_IF (_initialized_,
                 std::logic_error,
                 "Database manager is already initialized !");
    _services_ = &services_;
    return;
  }

  void manager::clear_plugins ()
  {
    _plugins_.clear();
    return;
  }

  manager::manager ()
  {
    _logging            = datatools::logger::PRIO_WARNING;
    _initialized_       = false;
    _services_ = 0;
    _plugins_factory_preload_ = true;
    _plugins_force_initialization_at_load_ = false;
    return;
  }

  manager::~manager()
  {
    if (_initialized_) {
      reset();
    }
    clear_plugins();
    reset_services();
    return;
  }

  void manager::init (const datatools::properties & config_)
  {
    this->initialize(config_);
    return;
  }

  void manager::initialize (const datatools::properties & config_)
  {
    DT_LOG_TRACE_ENTERING(_logging);
    DT_THROW_IF(_initialized_,
                std::logic_error,
                "Database manager is already initialized !");
    DT_LOG_TRACE(_logging, " Pre-init...");
    _pre_init(config_);
    DT_LOG_TRACE(_logging, " At-init...");
    _at_init_(config_);
    DT_LOG_TRACE(_logging, " Post-init...");
    _post_init(config_);
    _initialized_ = true;
    DT_LOG_TRACE_EXITING(_logging);
    return;
  }

  void manager::reset ()
  {
    DT_LOG_TRACE_ENTERING(_logging);
    DT_THROW_IF (! _initialized_,
                 std::logic_error,
                 "Database manager is not initialized ! Cannot reset !");
    _initialized_ = false;
    DT_LOG_TRACE_EXITING(_logging);
    return;
  }

  void manager::_init_plugins_ (const datatools::properties & config_)
  {
    typedef std::vector<std::string> PFList;
    typedef PFList::iterator PFListIterator;
    std::string conf_file_key("plugins.configuration_files");
    PFList conf_file_list;

    if (config_.has_key(conf_file_key)) {
      config_.fetch(conf_file_key, conf_file_list);
    }

    for (PFListIterator i = conf_file_list.begin();
         i < conf_file_list.end();
         ++i) {
      datatools::fetch_path_with_env(*i);
      datatools::multi_properties mconfig;
      mconfig.read(*i);
      this->load_plugins(mconfig);
    }
    return;
  }

  void manager::_at_init_ (const datatools::properties & config_)
  {
    std::string setup_label = _setup_label_;
    std::string setup_description = _setup_description_;
    std::string setup_version = _setup_version_;

    if (setup_label.empty()) {
      DT_THROW_IF (! config_.has_key ("setup_label"),
                   std::logic_error,
                   "Missing 'setup_label' property !");
      setup_label = config_.fetch_string ("setup_label");
    }

    if (setup_version.empty()) {
      if (config_.has_key ("setup_version")) {
        setup_version = config_.fetch_string ("setup_version");
      }
    }

    if (setup_description.empty()) {
      if (config_.has_key ("setup_description")) {
        setup_description = config_.fetch_string ("setup_description");
      }
    }

    {
      // Logging priority:
      datatools::logger::priority lp
        = datatools::logger::extract_logging_configuration (config_,
                                                            datatools::logger::PRIO_WARNING);
      DT_THROW_IF(lp == datatools::logger::PRIO_UNDEFINED,
                  std::logic_error,
                  "Invalid logging priority level for database manager !");
      set_logging_priority (lp);
    }

    if (config_.has_flag ("plugins.factory_no_preload")) {
      _plugins_factory_preload_ = false;
    }

    if (config_.has_flag ("plugins.force_initialization_at_load")) {
      _plugins_force_initialization_at_load_ = true;
    }

    // if (config_.has_key ("world_name")) {
    //   world_name = config_.fetch_string ("world_name");
    //   set_world_name (world_name);
    // }

    DT_LOG_DEBUG(_logging, "Properties are parsed...");

    // initializations:
    set_setup_label(setup_label);
    if (! setup_version.empty()) {
      set_setup_version(setup_version);
    }
    if (! setup_description.empty()) {
      set_setup_description(setup_description);
    }

    // // Plugins initialization :
    // DT_LOG_NOTICE(_logging, "Initialization of the database plugins... please wait...");
    // if (_plugins_factory_preload_) {
    //   DT_LOG_NOTICE(_logging, "Preloading plugin factory from system register...");
    //   this->_preload_plugins_global_dict();
    // }
    // _init_plugins_(config_);

    DT_LOG_NOTICE(_logging, "Exiting.");

    return;
  }


  bool manager::can_drop_plugin (const std::string& plugin_name_)
  {
    plugins_dict_type::const_iterator found = _plugins_.find(plugin_name_);
    DT_THROW_IF(found == _plugins_.end(),
                std::logic_error,
                "Database plugin '" << plugin_name_ << "' does not exist !");
    return found->second.can_be_dropped();
  }


  void manager::drop_plugin (const std::string& plugin_name_)
  {
    plugins_dict_type::iterator found = _plugins_.find(plugin_name_);
    DT_THROW_IF(found == _plugins_.end(),
                std::logic_error,
                "Database plugin '" << plugin_name_ << "' does not exist !");
    plugin_entry& entry = found->second;
    DT_THROW_IF(! found->second.can_be_dropped(),
                std::logic_error,
                "Plugin '" << plugin_name_
                << "' cannot be dropped because of some external constraints !");
    DT_LOG_DEBUG(_logging, "Reset & remove plugin '" << plugin_name_ << "' !");
    this->_reset_plugin(entry);
    _plugins_.erase(found);
  }

  void manager::print_plugins (std::ostream& out_,
                               const std::string& title_,
                               const std::string& indent_) const
  {
    std::string indent;
    if (!indent_.empty()) indent = indent_;

    if (!title_.empty()) out_ << title_ << ":" << std::endl;

    // Plugins:
    {
      size_t sz = _plugins_.size();
      size_t count = 0;
      for (plugins_dict_type::const_iterator it = _plugins_.begin();
           it != _plugins_.end();
           ++it) {
        count++;
        out_ << indent;
        if (count == sz) {
          out_ << "`-- ";
        } else {
          out_ << "|-- ";
        }
        const std::string& plugin_name = it->first;
        const plugin_entry& plugin_record = it->second;

        out_ << "Name : '" << plugin_name << "' "
             << "Type : '" << plugin_record._id_ << "' ";
        out_ << '(';
        int local_count = 0;
        if (plugin_record._status_ & plugin_entry::STATUS_INITIALIZED) {
          out_ << "initialized";
          local_count++;
        }
        out_ << ')';
        out_ << std::endl;
      }
    }
    return;
  }

  void manager::load_plugin(const std::string & plugin_name_,
                            const std::string & plugin_id_,
                            const datatools::properties & plugin_config_,
                            bool only_lock_)
  {
    DT_THROW_IF (only_lock_ && this->is_initialized(),
                 std::logic_error,
                 "Database manager is already initialized !");
    this->_load_plugin(plugin_name_, plugin_id_, plugin_config_);
  }

  void manager::load_plugins(const datatools::multi_properties & plugin_mconfig_,
                             bool only_lock_)
  {
    DT_LOG_TRACE(_logging, "Entering...");
    DT_THROW_IF(only_lock_ && this->is_initialized(),
                std::logic_error,
                "Database manager is already initialized !");
    for (datatools::multi_properties::entries_ordered_col_type::const_iterator i =
           plugin_mconfig_.ordered_entries().begin();
         i != plugin_mconfig_.ordered_entries().end();
         ++i) {
      datatools::multi_properties::entry* mpe = *i;
      const std::string& plugin_name = mpe->get_key();
      const std::string& plugin_id = mpe->get_meta();
      DT_LOG_NOTICE(_logging,
                    "Loading plugin named '"
                    << plugin_name << "' "
                    << " with ID '"
                    << plugin_id << "'...");
      this->_load_plugin(plugin_name, plugin_id, mpe->get_properties());
    }
  }


  //----------------------------------------------------------------------
  // Protected Interface Definitions
  //

  void manager::_pre_init (const datatools::properties & /*config_*/) {
  }

  void manager::_post_init (const datatools::properties & /*config_*/) {
  }


  void manager::_preload_plugins_global_dict()
  {
    DT_LOG_TRACE(_logging, "Entering...");
    _plugins_factory_register_.import(DATATOOLS_FACTORY_GET_SYSTEM_REGISTER(database::manager::base_plugin));
    return;
  }


  void manager::_load_plugin(const std::string& name_,
                             const std::string& id_,
                             const datatools::properties& config_)
  {
    DT_LOG_TRACE(_logging, "Entering...");
    DT_THROW_IF (this->has_plugin(name_),
                 std::logic_error,
                 "Plugin '" << name_ << "' already exists !");

    {
      // Add a new entry :
      plugin_entry tmp_entry;
      tmp_entry._name_ = name_;
      DT_LOG_DEBUG(_logging, "Add an entry for plugin '" << name_ << "'...");
      _plugins_[name_] = tmp_entry;
    }
    // fetch a reference on it and update :
    plugin_entry& new_entry = _plugins_.find(name_)->second;
    new_entry._id_     = id_;
    new_entry._config_ = config_;
    new_entry._status_ = plugin_entry::STATUS_BLANK;
    //DT_LOG_DEBUG(_logging, "Fetch...");
    this->_create_plugin(new_entry);

    if (_plugins_force_initialization_at_load_) {
      this->_initialize_plugin(new_entry);
    }
    DT_LOG_TRACE(_logging, "Exiting.");
    return;
  }


  void manager::_create_plugin(plugin_entry& entry_)
  {
    if (!(entry_._status_ & plugin_entry::STATUS_CREATED)) {
      DT_LOG_DEBUG(_logging, "Creating plugin named '" <<  entry_._name_ << "'...");
      // search for the plugin's label in the factory dictionary:
      DT_THROW_IF(!_plugins_factory_register_.has(entry_._id_),
                  std::logic_error,
                  "Cannot find plugin factory with ID '" << entry_._id_
                  << "' for plugin named '"
                  << entry_._name_ << "' !");
      typedef database::manager::base_plugin::factory_register_type::factory_type FactoryType;
      const FactoryType& the_factory = _plugins_factory_register_.get(entry_._id_);
      base_plugin* ptr = the_factory();
      entry_._handle_.reset(ptr);
      entry_._status_ |= plugin_entry::STATUS_CREATED;
      DT_LOG_DEBUG(_logging,
                   "Plugin named '"
                   <<  entry_._name_
                   << "' has been created !");
    }
    return;
  }


  void manager::_initialize_plugin(plugin_entry& entry_)
  {
    // If not created, first do it :
    if (!(entry_._status_ & plugin_entry::STATUS_CREATED)) {
      this->_create_plugin(entry_);
    }

    // If not initialized, do it :
    if (!(entry_._status_ & plugin_entry::STATUS_INITIALIZED)) {
      DT_LOG_DEBUG(_logging,
                   "Initializing plugin named '"
                   << entry_._name_
                   << "'...");
      base_plugin& the_plugin = entry_._handle_.grab();
      the_plugin.set_db_manager(*this);
      if (_services_ == 0) {
        the_plugin.initialize_simple (entry_._config_, _plugins_);
      } else {
        the_plugin.initialize (entry_._config_, _plugins_, *_services_);
      }
      entry_._status_ |= plugin_entry::STATUS_INITIALIZED;
      DT_LOG_NOTICE(_logging,
                    "Plugin named '"
                    << entry_._name_
                    << "' has been initialized.");
    }
    return;
  }


  void manager::_reset_plugin(plugin_entry& entry_)
  {
    if (entry_._status_ & plugin_entry::STATUS_INITIALIZED) {
      base_plugin& the_plugin = entry_._handle_.grab();
      the_plugin.reset();
      entry_._status_ ^= plugin_entry::STATUS_INITIALIZED;
    }
    return;
  }

  void manager::tree_dump(std::ostream& out_,
                          const std::string& title_,
                          const std::string& indent_,
                          bool inherit_) const
  {

    std::string indent;
    if (!indent_.empty()) indent = indent_;

    if (!title_.empty()) out_ << indent << title_ << std::endl;

    out_ << indent << datatools::i_tree_dumpable::tag
        << "Logging              : '"
        << datatools::logger::get_priority_label(_logging) << "'"
        << std::endl;

    out_ << indent << datatools::i_tree_dumpable::tag
        << "Initialized          : "
        << _initialized_
        << "" << std::endl;

    out_ << indent << datatools::i_tree_dumpable::tag
        << "Setup label          : '"
        << _setup_label_
        << "'" << std::endl;

    out_ << indent << datatools::i_tree_dumpable::tag
        << "Setup version        : '"
        << _setup_version_
        << "'" << std::endl;

    out_ << indent << datatools::i_tree_dumpable::tag
        << "Setup description    : '"
        << _setup_description_
        << "'" << std::endl;

    out_ << indent << datatools::i_tree_dumpable::tag
        << "Services dictionnary : "
        << _services_
        << " " << std::endl;

    out_ << indent << datatools::i_tree_dumpable::tag
         << "Shape factory : "
         << " " << std::endl;

    out_ << indent << datatools::i_tree_dumpable::tag
         << "Plugins factory preload              : "
         << _plugins_factory_preload_
         << " " << std::endl;

    out_ << indent << datatools::i_tree_dumpable::tag
         << "Plugins force initialization at load : "
         << _plugins_force_initialization_at_load_
         << " " << std::endl;

    out_ << indent << datatools::i_tree_dumpable::tag
         << "Plugins factories                    : "
         << std::endl;
    {
      std::ostringstream indent2;
      indent2 << indent <<  datatools::i_tree_dumpable::skip_tag;
      std::vector<std::string> plugin_factory_names;
      _plugins_factory_register_.tree_dump(out_,"",indent2.str());
      /*
      std::vector<std::string> plugin_factory_names;
      _plugins_factory_register_.list_of_factories(plugin_factory_names);
      for (int i = 0; i < plugin_factory_names.size(); i++)  {
        int j = i;
        j++;
        out_ << indent;
        out_ << datatools::i_tree_dumpable::skip_tag;
        if (j == plugin_factory_names.size()) {
          out_ << datatools::i_tree_dumpable::last_tag;
        } else {
          out_ << datatools::i_tree_dumpable::tag;
        }
        out_ << plugin_factory_names[i] << std::endl;
      }
      */
    }

    out_ << indent << datatools::i_tree_dumpable::inherit_tag(inherit_)
        << "Plugins                              : "
        << _plugins_.size()
        << " " << std::endl;
    for (plugins_dict_type::const_iterator i = _plugins_.begin();
         i != _plugins_.end();
         i++) {
      plugins_dict_type::const_iterator j = i;
      j++;
      out_ << indent;
      out_ << datatools::i_tree_dumpable::inherit_skip_tag(inherit_);
      if (j == _plugins_.end()) {
        out_ << datatools::i_tree_dumpable::last_tag;
      } else {
        out_ << datatools::i_tree_dumpable::tag;
      }
      out_ << "'" << i->first << "' as a '" << i->second.get_id() << "'";
      if (i->second.is_created()) {
        out_ << " (created)";
      } else if  (i->second.is_initialized()) {
        out_ << " (initialized)";
      }
      out_ << std::endl;
    }

    return;
  }

  const manager::base_plugin::factory_register_type & manager::get_plugins_factory_register()
  {
    return _plugins_factory_register_;
  }

  //----------------------------------------------------------------------
  // Private Interface Definitions

  /*
    void manager::_set_plugins_factory_preload_(bool preload_)
    {
    _plugins_factory_preload_ = preload_;
    if (preload_) {
    this->_preload_plugins_global_dict();
    }
    return;
    }
  */

} // end of namespace database

/****************************************************************/
// OCD support for class '::database::manager' :
DOCD_CLASS_IMPLEMENT_LOAD_BEGIN(::database::manager,ocd_)
{
  ocd_.set_class_name("database::manager");
  ocd_.set_class_description("A database manager");
  ocd_.set_class_library("database");
  ocd_.set_class_documentation("The database manager is ...                                     \n"
                               );


  {
    configuration_property_description & cpd = ocd_.add_configuration_property_info();
    cpd.set_name_pattern("logging.priority")
      .set_terse_description("The logging priority threshold")
      .set_traits(datatools::TYPE_STRING)
      .set_mandatory(false)
      .add_example("Activate the warning logging level::        \n"
                   "                                            \n"
                   "    logging.priority : string = \"warning\" \n"
                   "                                            \n"
                   )
      ;
  }

  // {
  //   configuration_property_description & cpd = ocd_.add_configuration_property_info();
  //   cpd.set_name_pattern("debug")
  //     .set_terse_description("The debug flag")
  //     .set_traits(datatools::TYPE_BOOLEAN)
  //     .set_mandatory(false)
  //     .set_long_description("The debug flag activates some debug printing.    \n"
  //                           "Not recommended for a production run.            \n"
  //                           "Example :                                        \n"
  //                           "                                                 \n"
  //                           "    debug : boolean = 0                          \n"
  //                           "                                                 \n"
  //                           )
  //     ;
  // }

  // {
  //   configuration_property_description & cpd = ocd_.add_configuration_property_info();
  //   cpd.set_name_pattern("verbose")
  //     .set_terse_description("The verbose flag")
  //     .set_traits(datatools::TYPE_BOOLEAN)
  //     .set_mandatory(false)
  //     .set_long_description("The verbose flag activates some additionnal printing.\n"
  //                           "Not recommended for a production run.            \n"
  //                           "Example :                                        \n"
  //                           "                                                 \n"
  //                           "    verbose : boolean = 0                        \n"
  //                           "                                                 \n"
  //                           )
  //     ;
  // }

  {
    configuration_property_description & cpd = ocd_.add_configuration_property_info();
    cpd.set_name_pattern("setup_label")
      .set_terse_description("The label that identifies the database setup")
      .set_traits(datatools::TYPE_STRING)
      .set_mandatory(true)
      .set_complex_triggering_conditions(true)
      .set_long_description("Superseded by a previous call to :            \n"
                            "  ``database::manager::set_setup_label(...)`` \n")
      .add_example("Set the label of the virtual database setup::  \n"
                   "                                               \n"
                   "  setup_label : string = \"test_setup\"        \n"
                   "                                               \n"
                   )
      ;
  }

  {
    configuration_property_description & cpd = ocd_.add_configuration_property_info();
    cpd.set_name_pattern("setup_version")
      .set_terse_description("The version string of the virtual database setup")
      .set_traits(datatools::TYPE_STRING)
      .set_mandatory(false)
      .set_long_description("Superseded by a previous call to:               \n"
                            "  ``database::manager::set_setup_version(...)`` \n")
      .add_example("Set the version of the virtual database setup::  \n"
                   "                                                 \n"
                   "    setup_version : string = \"0.1\"             \n"
                   "                                                 \n"
                   )
      ;
  }

  {
    configuration_property_description & cpd = ocd_.add_configuration_property_info();
    cpd.set_name_pattern("setup_description")
      .set_terse_description("The description string of the virtual database setup")
      .set_traits(datatools::TYPE_STRING)
      .set_mandatory(false)
      .set_long_description("Superseded by a previous call to :                  \n"
                            "  ``database::manager::set_setup_description(...)`` \n")
      .add_example("Set the description of the virtual database setup::  \n"
                   "                                                     \n"
                   "    setup_description : string = \"My test setup\"   \n"
                   "                                                     \n"
                   )
      ;
  }

  {
    configuration_property_description & cpd = ocd_.add_configuration_property_info();
    cpd.set_name_pattern("plugins.factory_no_preload")
      .set_terse_description("The flag inhibits the preloading of plugins' factories")
      .set_traits(datatools::TYPE_BOOLEAN)
      .set_mandatory(false)
      .set_default_value_boolean(false)
      .set_long_description("This flag inhibits the preloading of plugins' factories  \n"
                            "from the plugins system register.                        \n"
                            "Not recommended for a production run.                    \n")
      .add_example("Use the plugin factory preload::                         \n"
                   "                                                         \n"
                   "    plugins.factory_no_preload : boolean = 0             \n"
                   "                                                         \n"
                   )
      ;
  }

  {
    configuration_property_description & cpd = ocd_.add_configuration_property_info();
    cpd.set_name_pattern("plugins.force_initialization_at_load")
      .set_terse_description("The flag to force the initialization of plugins at load stage.")
      .set_traits(datatools::TYPE_BOOLEAN)
      .set_mandatory(false)
      .set_long_description("This flag triggers the initialization of plugins as soons as \n"
                            "they are loaded in the database manager's plugin system.     \n"
                            "Default is false to activate the dedicated resources of a    \n"
                            "given plugin if and only if the plugin is used.              \n"
                            "Not recommended for a production run.                        \n")
      .set_default_value_boolean(false)
      .add_example("Do not force initialization at load step::           \n"
                   "                                                     \n"
                   "  plugins.force_initialization_at_load : boolean = 0 \n"
                   "                                                     \n"
                   )
      ;
  }


  ocd_.set_configuration_hints("A database manager is configured through a configuration file that  \n"
                               "uses the format of 'datatools::properties' setup file.              \n"
                               "Example: ::         .                                               \n"
                               "  logging.priority : string = \"warning\"                           \n"
                               "  setup_label : string = \"Foo\"                                    \n"
                               "  setup_version : string = \"1.2\"                                  \n"
                               "  setup_description : string = \\                                   \n"
                               "     \"The virtual database setup of the Foo experiment\"           \n"
                               "                                                                   \n"
                              );

  ocd_.set_validation_support(true);
  ocd_.lock();
  return;
}
DOCD_CLASS_IMPLEMENT_LOAD_END()
DOCD_CLASS_SYSTEM_REGISTRATION(::database::manager,"database::manager")
