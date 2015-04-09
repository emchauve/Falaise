// snemo/digitization/geiger_signal.cc
// Author(s): Yves LEMIERE <lemiere@lpccaen.in2p3.fr>
// Author(s): Guillaume OLIVIERO <goliviero@lpccaen.in2p3.fr>

// Ourselves:
#include <snemo/digitization/geiger_signal.h>

// Third party:
// - Bayeux/datatools:
#include <datatools/exception.h>
#include <datatools/clhep_units.h>
#include <datatools/utils.h>

namespace snemo {
  
  namespace digitization {

    // Serial tag for datatools::serialization::i_serializable interface :
    // DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(geiger_signal, "snemo::digitalization::geiger_signal")
    
    geiger_signal::geiger_signal()
    {
      _locked_ = false;
      datatools::invalidate(_anode_avalanche_time_);
      datatools::invalidate(_plasma_top_time_);
      datatools::invalidate(_plasma_bottom_time_);
      return;
    }

    geiger_signal::~geiger_signal()
    {
      reset();
      return;
    }

    void geiger_signal::set_header(int32_t geiger_signal_hit_id_,
		    const geomtools::geom_id & electronic_id_)
    {
      set_hit_id(geiger_signal_hit_id_);
      set_geom_id(electronic_id_);
      return;
    }
			
    void geiger_signal::set_data(const double & anode_avalanche_time_)
    {
      set_anode_avalanche_time(anode_avalanche_time_);
      return;
    }
       
    double geiger_signal::get_anode_avalanche_time() const
    {
      return _anode_avalanche_time_;
    }
      
    void geiger_signal::set_anode_avalanche_time(const double & anode_avalanche_time_)
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Geiger signal is locked ! ");
      _anode_avalanche_time_ = anode_avalanche_time_;
      return;
    }
   
    double geiger_signal::get_plasma_top_time_() const
    {
      return _plasma_top_time_;
    }

    void geiger_signal::set_plasma_top_time_(const double & plasma_top_time_)
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Geiger signal is locked ! ");
      _plasma_top_time_ = plasma_top_time_;
      return;
    }
   
    double geiger_signal::get_plasma_bottom_time_() const
    {
      return _plasma_bottom_time_;
    }
    
    void geiger_signal::set_plasma_bottom_time_(const double & plasma_bottom_time_)
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Geiger signal is locked ! ");
      _plasma_bottom_time_ = plasma_bottom_time_;
      return;
    }

    bool geiger_signal::is_locked() const
    {
      return _locked_;
    }

    void geiger_signal::lock()
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Geiger signal is already locked ! ");
      _check();
      _locked_ = true;
      return;
    }
    
    void geiger_signal::unlock()
    {
      DT_THROW_IF(!is_locked(), std::logic_error, "Geiger signal is already unlocked ! ");
      _locked_ = false;
      return;
    } 

    bool geiger_signal::is_valid() const
    {
      return true;
    }

    void geiger_signal::reset()
    {
      if(is_locked())
	{
	  unlock();
	}
      geomtools::base_hit::reset();
      return;
    }

    void geiger_signal::tree_dump (std::ostream & out_,
				   const std::string & title_,
				   const std::string & indent_,
				   bool inherit_) const
    {
      base_hit::tree_dump (out_, title_, indent_, true);

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Avalanche anode time =  : " << _anode_avalanche_time_ / CLHEP::microsecond << " us " << std::endl;

      return;
    }
    
    void geiger_signal::_check()
    {
      DT_THROW_IF(!is_valid(), std::logic_error, " is not valid ! ");
    }


  } // end of namespace digitization

} // end of namespace snemo
