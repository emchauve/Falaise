// falaise/snemo/datamodels/precalibrated_data.cc

// Ourselves:
#include <falaise/snemo/datamodels/precalibrated_data.h>

// Third party:
// - Bayeux/datatools
#include <datatools/clhep_units.h>

namespace snemo {

  namespace datamodel {

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(precalibrated_data,
                                                      "snemo::datamodel::precalibrated_data")

    const PreCalibratedCalorimeterHitHdlCollection& precalibrated_data::calorimeter_hits() const {
      return calorimeter_hits_;
    }

    PreCalibratedCalorimeterHitHdlCollection& precalibrated_data::calorimeter_hits() { return calorimeter_hits_; }

    const PreCalibratedTrackerHitHdlCollection& precalibrated_data::tracker_hits() const { return tracker_hits_; }

    PreCalibratedTrackerHitHdlCollection& precalibrated_data::tracker_hits() { return tracker_hits_; }

    void precalibrated_data::clear() {
      calorimeter_hits_.clear();
      tracker_hits_.clear();
      _properties_.clear();
    }

    void precalibrated_data::tree_dump(std::ostream& out, const std::string& title,
                                       const std::string& indent, bool is_last) const {
      auto printCaloHit = [&](const PreCalibratedCalorimeterHitHdl& x) {
        out << "Id: " << x->get_hit_id() << ", GID: " << x->get_geom_id()
	    << ", Baseline: " << x->get_baseline() / (1E-3 * CLHEP::volt) << " mV"
	    << ", Amplitude: " << x->get_amplitude() / (1E-3 * CLHEP::volt) << " mV"
	    << ", Charge: " << x->get_charge() / (CLHEP::volt * CLHEP::second * 1E-9) << " nV.s"
	    << ", Time: " << x->get_time() / CLHEP::second << " s" << std::endl;
      };

      auto printPreCalibratedTrackerHit = [&](const PreCalibratedTrackerHitHdl& x) {
        out << "Id: " << x->get_hit_id() << ", GID: " << x->get_geom_id()
	    << ", AnodicTime = " << x->get_anodic_time() / CLHEP::second << " s" << std::endl;
      };

      if (!title.empty()) {
        out << indent << title << std::endl;
      }

      // Precalibrated calorimeter hits:
      out << indent << datatools::i_tree_dumpable::tag << "PreCalibratedCalorimeterHits[" << calorimeter_hits_.size()
          << "]:" << std::endl;
      if (!calorimeter_hits_.empty()) {
        std::for_each(
                      calorimeter_hits_.begin(), --calorimeter_hits_.end(), [&](const PreCalibratedCalorimeterHitHdl& x) {
                        out << indent << datatools::i_tree_dumpable::skip_tag << datatools::i_tree_dumpable::tag;
                        printCaloHit(x);
                      });
        out << indent << datatools::i_tree_dumpable::skip_tag << datatools::i_tree_dumpable::last_tag;
        printCaloHit(calorimeter_hits_.back());
      }

      // Precalibrated tracker hits:
      out << indent << datatools::i_tree_dumpable::inherit_tag(is_last) << "PreCalibratedTrackerHits["
          << tracker_hits_.size() << "]:" << std::endl;

      if (!tracker_hits_.empty()) {

        std::for_each(tracker_hits_.begin(), --tracker_hits_.end(), [&](const PreCalibratedTrackerHitHdl& x) {
            out << indent << datatools::i_tree_dumpable::inherit_skip_tag(is_last)
                << datatools::i_tree_dumpable::tag;
            printPreCalibratedTrackerHit(x);
          });
        out << indent << datatools::i_tree_dumpable::inherit_skip_tag(is_last)
            << datatools::i_tree_dumpable::last_tag;
        printPreCalibratedTrackerHit(tracker_hits_.back());
      }
    }

  }  // end of namespace datamodel

}  // end of namespace snemo
