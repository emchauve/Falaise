// -*- mode: c++ ; -*-
/// \file falaise/snemo/datamodels/particle_track_data.ipp

#ifndef FALAISE_SNEMO_DATAMODEL_PARTICLE_TRACK_DATA_IPP
#define FALAISE_SNEMO_DATAMODEL_PARTICLE_TRACK_DATA_IPP 1

// Ourselves:
#include <falaise/snemo/datamodels/particle_track_data.h>

// Third party:
// - Boost:
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>
// - Bayeux/datatools:
#include <datatools/properties.ipp>

// This project:
#include <falaise/snemo/datamodels/boost_io/particle_track.ipp>

namespace snemo {

namespace datamodel {

template <class Archive>
void particle_track_data::serialize(Archive& ar_, const unsigned int /*version_*/) {
  ar_& DATATOOLS_SERIALIZATION_I_SERIALIZABLE_BASE_OBJECT_NVP;
  ar_& boost::serialization::make_nvp("particles", particles_);
  ar_& boost::serialization::make_nvp("non_associated_calorimeters", isolated_calorimeters_);
  ar_& boost::serialization::make_nvp("auxiliaries", _auxiliaries_);
}

}  // end of namespace datamodel

}  // end of namespace snemo

#endif  // FALAISE_SNEMO_DATAMODEL_PARTICLE_TRACK_DATA_IPP
