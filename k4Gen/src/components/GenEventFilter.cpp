// Gaudi
#include <Gaudi/Property.h>
#include <Gaudi/Functional/FilterPredicate.h>

// k4FWCore
#include "k4FWCore/BaseClass.h"

// ROOT
#include "TInterpreter.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TGlobal.h"

// EDM4hep
#include "edm4hep/MCParticleCollection.h"

struct GenEventFilter final : Gaudi::Functional::FilterPredicate<bool(const edm4hep::MCParticleCollection&), BaseClass_t> {
  GenEventFilter(const std::string& name, ISvcLocator* pSvc) : FilterPredicate( name, pSvc, {
      KeyValue("particles", "input particle collection")}),
                                                               m_nEventsAccepted(0),
                                                               m_nEventsSeen(0),
                                                               m_nEventsSkipped(0),
                                                               m_filterRulePtr(nullptr) {}


  StatusCode initialize() {
    SmartIF<IProperty> appMgr;
    appMgr = service("ApplicationMgr", appMgr);
    Gaudi::Property<int> evtMax;
    evtMax.assign(appMgr->getProperty("EvtMax"));
    m_nEventsTarget = evtMax;
    debug() << "Targeted number of events: " << m_nEventsTarget << endmsg;

    if (!m_filterRuleStr.value().empty() && !m_filterRulePath.value().empty()) {
      throw std::runtime_error(
        "Multiple filter rules found!\n"
        "Provide either a string or a cxx/hxx file, not both.");
    }

    // Filter rule provided in a C++ file
    if (!m_filterRulePath.value().empty()) {
      debug() << "Looking for the filter rule in " << m_filterRulePath.value()
              << endmsg;
      if (gSystem->AccessPathName(m_filterRulePath.value().c_str())) {
        throw std::runtime_error(
          "Unable to access filter rule file!\n"
          "Provided filter rule file path: " + m_filterRulePath.value());
      }
      // Include and compile the file
      {
        bool success = gInterpreter->Declare(
            ("#include \"" + m_filterRulePath + "\"").c_str());
        if (!success) {
          throw std::runtime_error(
            "Unable to compile filter rule provided in a file!");
        }
        debug() << "Filter rule provided in a file compiled successfully."
                << endmsg;
      }
    }

    // Filter rule provided directly in a string
    if (!m_filterRuleStr.value().empty()) {
      debug() << "Compiling filter rule from a string..." << endmsg;
      {
        // Necessary include
        bool success = gInterpreter->Declare(
            "#include \"edm4hep/MCParticleCollection.h\"");
        if (!success) {
          throw std::runtime_error(
              "Unable to find edm4hep::MCParticleCollection header file!");
        }
        debug() << "Found edm4hep::MCParticleCollection header file." << endmsg;
      }

      {
        bool success = gInterpreter->Declare(m_filterRuleStr.value().c_str());
        if (!success) {
          throw std::runtime_error(
              "Unable to compile filter rule provided in a string!");
        }
        debug() << "Filter rule provided in a string compiled successfully."
                << endmsg;
      }
    }

    if (m_filterRuleStr.value().empty() && m_filterRulePath.value().empty()) {
      warning() << "No filter rule provided, all events will be accepted!"
                << endmsg;
      gInterpreter->Declare(
        "#include \"edm4hep/MCParticleCollection.h\"\n"
        "bool filterRule(const edm4hep::MCParticleCollection&){return true;}");
    }

    // Get the address of the compiled filter rule from the interpreter
    {
      enum TInterpreter::EErrorCode err = TInterpreter::kProcessing;
      m_filterRulePtr =
        reinterpret_cast<bool (*)(const edm4hep::MCParticleCollection&)>(
          gInterpreter->ProcessLineSynch("&filterRule", &err));
      if (err != TInterpreter::kNoError) {
        throw std::runtime_error("Unable to obtain filter rule pointer!");
      }
      debug() << "Filter rule pointer obtained successfully." << endmsg;
    }

    // Check if the filter rule pointer has correct signature
    {
      auto success = gInterpreter->Declare("auto filterRulePtr = &filterRule;");
      if (!success) {
        throw std::runtime_error(
          "Unable to declare filter rule pointer in the interpreter!");
      }
      auto global = gROOT->GetGlobal("filterRulePtr");
      if (!global) {
        throw std::runtime_error(
          "Unable to obtain filter rule pointer from the interpreter!");
      }
      std::string filterRuleType = global->GetTypeName();
      if (filterRuleType != "bool(*)(const edm4hep::MCParticleCollection&)") {
        throw std::runtime_error(
          "Found filter rule pointer has wrong signature!\n"
          "Required: bool(*)(const edm4hep::MCParticleCollection&)\n"
          "Found: " + filterRuleType);
      }
      debug() << "Found filter rule pointer has correct signature." << endmsg;
    }

    return Gaudi::Algorithm::initialize();
  }

  bool operator()(const edm4hep::MCParticleCollection& inParticles) const override {
    m_nEventsSeen++;

    if (!(*m_filterRulePtr)(inParticles)) {
      m_nEventsSkipped++;
      debug() << "Skipping event (" << m_nEventsSkipped << ") ..." << endmsg;
      return false;
    }

    m_nEventsSkipped = 0;
    m_nEventsAccepted++;

    debug() << "Event contains " << inParticles.size() << " particles."
            << endmsg;
    debug() << "Number of events seen so far: " << m_nEventsSeen << endmsg;
    debug() << "Targeted number of events: " << m_nEventsTarget
            << endmsg;
    debug() << "Number of events already accepted: " << m_nEventsAccepted
            << endmsg;
    debug() << "Remaining number of events to get to the target: "
            << m_nEventsTarget.load() - m_nEventsAccepted.load() << endmsg;

    return true;
  }

  StatusCode finalize() {
    debug() << "Number of events seen: " << m_nEventsSeen << endmsg;
    debug() << "Number of events accepted: " << m_nEventsAccepted << endmsg;
    debug() << "Number of events targeted: " << m_nEventsTarget << endmsg;

    return Gaudi::Algorithm::finalize();
  }

  /// Targeted number of events.
  mutable std::atomic<unsigned int> m_nEventsTarget;
  /// Keep track of how many events were already accepted.
  mutable std::atomic<unsigned int> m_nEventsAccepted;
  /// Keep track of how many events we went through.
  mutable std::atomic<unsigned int> m_nEventsSeen;
  /// How many events were skipped in a row.
  mutable std::atomic<unsigned int> m_nEventsSkipped;

  private:
    /// Rule to filter the events with
    Gaudi::Property<std::string> m_filterRuleStr{
        this, "filterRule", "", "Filter rule to apply on the events"};

    /// Path of the filter rule file
    Gaudi::Property<std::string> m_filterRulePath{
        this, "filterRulePath", "", "Path to the filter rule file"};

    /// Filter rule pointer.
    bool (*m_filterRulePtr)(const edm4hep::MCParticleCollection&);
};

DECLARE_COMPONENT( GenEventFilter );
