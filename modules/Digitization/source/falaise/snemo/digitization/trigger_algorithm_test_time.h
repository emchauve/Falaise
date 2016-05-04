// snemo/digitization/trigger_algorithm_test_time.h
// Author(s): Yves LEMIERE <lemiere@lpccaen.in2p3.fr>
// Author(s): Guillaume OLIVIERO <goliviero@lpccaen.in2p3.fr>

#ifndef FALAISE_DIGITIZATION_PLUGIN_SNEMO_DIGITIZATION_TRIGGER_ALGORITHM_TEST_TIME_H
#define FALAISE_DIGITIZATION_PLUGIN_SNEMO_DIGITIZATION_TRIGGER_ALGORITHM_TEST_TIME_H

// Standard library :
#include <string>
#include <bitset>

// This project :
#include <snemo/digitization/calo_ctw_data.h>
#include <snemo/digitization/geiger_ctw_data.h>
#include <snemo/digitization/calo_trigger_algorithm_test_time.h>
#include <snemo/digitization/tracker_trigger_algorithm_test_time.h>
#include <snemo/digitization/coincidence_trigger_algorithm_test_time.h>
#include <snemo/digitization/electronic_mapping.h>
#include <snemo/digitization/mapping.h>
#include <snemo/digitization/trigger_display_manager.h>

namespace datatools {
  class properties;
}    

namespace snemo { 
  
  namespace digitization {

    /// \brief Full trigger algorithm for the process. The trigger decision is taken here.
    class trigger_algorithm_test_time
    {
 		public : 

			enum L2_trigger_mode{
				INVALID   = 0,
				CALO_ONLY = 1,
				CALO_TRACKER_TIME_COINC = 2,
				CARACO  = 3,
				OPEN_DELAYED = 4,
				APE     = 5,
				DAVE    = 6					
			};

			struct L1_calo_decision
			{
				L1_calo_decision();
				void reset();
				const void display() const;
				bool L1_calo_decision_bool;
				uint32_t L1_calo_ct_decision; // CT @ 25 ns
			};

			struct L1_tracker_decision
			{
				L1_tracker_decision();
				void reset();
				const void display() const;
				bool L1_tracker_decision_bool;
				uint32_t L1_tracker_ct_decision; // CT @ 1600 ns
			};
			
			struct L2_decision
			{
				L2_decision();
				void reset();
				const void display() const;
				bool L2_decision_bool;
				uint32_t L2_clocktick_decision;
				trigger_algorithm_test_time::L2_trigger_mode L2_trigger_mode;				
			};

			/// Previous event structure who pass the caraco trigger, store trigger informations (total = 173 [bits]). Useful to search delayed alpha pattern.
			struct previous_event_record
			{
				previous_event_record();
				void reset();
				const void display() const;
				// PER information :
				uint32_t previous_clocktick_1600ns;
				uint32_t counter_1600ns;
				// Calorimeter information :
				std::bitset<trigger_info::NZONES> calo_zoning_word[mapping::NUMBER_OF_SIDES];
				std::bitset<calo::ctw::HTM_BITSET_SIZE> total_multiplicity_side_0;
				std::bitset<calo::ctw::HTM_BITSET_SIZE> total_multiplicity_side_1;
				bool LTO_side_0;
				bool LTO_side_1;
				std::bitset<calo::ctw::HTM_BITSET_SIZE> total_multiplicity_gveto;
				bool LTO_gveto;
				std::bitset<calo_trigger_algorithm_test_time::XT_INFO_BITSET_SIZE> xt_info_bitset;
				bool single_side_coinc;
				bool total_multiplicity_threshold;
				// Tracker information :
				std::bitset<trigger_info::DATA_FULL_BITSET_SIZE> tracker_finale_data_per_zone[trigger_info::NSIDES][trigger_info::NZONES];

				// Coincidence zoning or tracker zoning ?
				std::bitset<trigger_info::NZONES> zoning_word[mapping::NUMBER_OF_SIDES];
			};

			/// Trigger display manager is a friend because it can access to members for display
			friend class trigger_display_manager;
			
      /// Default constructor
      trigger_algorithm_test_time();

      /// Destructor
      virtual ~trigger_algorithm_test_time();

      /// Set the electronic mapping object
      void set_electronic_mapping(const electronic_mapping & my_electronic_mapping_);

      /// Check if the coincidence config is activated 
			bool is_activated_coincidence() const;

      /// Initializing
      void initialize();

      /// Initializing
      void initialize(const datatools::properties & config_);

      /// Check if the algorithm is initialized 
      bool is_initialized() const;

      /// Reset the object
      void reset(); 
	
			/// Clear the record vectors
			void clear_records();

			/// Get the vector of tracker record
			const std::vector<tracker_trigger_algorithm_test_time::tracker_record> get_tracker_records_vector() const;
			
			/// Get the vector of calo summary record
			const std::vector<calo_trigger_algorithm_test_time::calo_summary_record> get_calo_records_vector() const;

			/// Get the vector of coincidence record
			const std::vector<coincidence_trigger_algorithm_test_time::coincidence_calo_record> get_coincidence_calo_records_vector() const;
			
			/// Get the vector of coincidence record
			const std::vector<coincidence_trigger_algorithm_test_time::coincidence_event_record> get_coincidence_records_vector() const;
 			
			/// Get the finale trigger decision
			const bool get_finale_decision() const;	

			/// Get the finale trigger decision
			const bool get_delayed_finale_decision() const;
					
      /// General process
      void process(const calo_ctw_data & calo_ctw_data_,
									 const geiger_ctw_data & geiger_ctw_data_);

    protected :
			
			/// Process calo algorithm
			void _process_calo_algo(const calo_ctw_data & calo_ctw_data_);
			
			/// Process tracker algorithm
			void _process_tracker_algo(const geiger_ctw_data & geiger_ctw_data_);
			
			/// Process coincidence module algorithm			
			void _process_coinc_algo();
			
      /// Protected general process
      void _process(const calo_ctw_data & calo_ctw_data_,
										const geiger_ctw_data & geiger_ctw_data_);

    private :
     
			typedef boost::circular_buffer<trigger_algorithm_test_time::previous_event_record> buffer_previous_event_record_type;

      // Configuration :
      bool _initialized_; //!< Initialization flag
      const electronic_mapping * _electronic_mapping_; //!< Convert geometric ID into electronic ID
			
			// Trigger configuration :			
			bool _activate_calorimeter_only_;
			bool _activate_calo_tracker_time_coincidence_;
			bool _activate_caraco_;
			bool _activate_take_all_delayed_;
			bool _activate_ape_dave_coincidence_;
			bool _activate_ape_coincidence_only_;
			bool _activate_coincidence_; //!< Boolean activating coincidence



			// Trigger algorithms :
		  tracker_trigger_algorithm_test_time      _tracker_algo_; //!< Tracker trigger algorithm @ 1600ns
		  calo_trigger_algorithm_test_time         _calo_algo_; //!< Calo trigger algorithm @ 25ns
			coincidence_trigger_algorithm_test_time  _coinc_algo_; //!< Coincidence trigger algorithm for matching calo and tracker trigger
			
			// Data :
			std::vector<calo_trigger_algorithm_test_time::calo_summary_record> _calo_records_; //!< Collection of calo summary record @ 25 ns
			std::vector<coincidence_trigger_algorithm_test_time::coincidence_calo_record> _coincidence_calo_records_; //!< Collection of coincidence calo records @ 1600 ns
			std::vector<tracker_trigger_algorithm_test_time::tracker_record> _tracker_records_; //!< Collection of tracker record @ 1600 ns
			std::vector<coincidence_trigger_algorithm_test_time::coincidence_event_record> _coincidence_records_; //!< Collection of coincidence record @ 1600 ns
			boost::scoped_ptr<buffer_previous_event_record_type> _previous_event_records_; //!< Collection of previous event records @ 1600 ns
			std::vector<trigger_algorithm_test_time::L1_calo_decision> _L1_calo_decision_records_; //!< Collection of L1 calorimeter decision
			std::vector<trigger_algorithm_test_time::L1_tracker_decision> _L1_tracker_decision_records_; //!< Collection of L1 tracker decision
			std::vector<trigger_algorithm_test_time::L2_decision> _L2_decision_records_; //!< Collection of L2 decision (which launch the readout)
			
			
			bool _finale_trigger_decision_; //!< The finale decision for the trigger
			bool _delayed_finale_trigger_decision_; //!< The finale decision for the trigger
			
    };

  } // end of namespace digitization

} // end of namespace snemo

#endif // FALAISE_DIGITIZATION_PLUGIN_SNEMO_DIGITIZATION_TRIGGER_ALGORITHM_TEST_TIME_H

/* 
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
