// Copyright © 2022-2023 GothicKit Contributors.
// SPDX-License-Identifier: MIT
#pragma once
#include "zenkit/Library.hh"
#include "zenkit/Misc.hh"
#include "zenkit/vobs/VirtualObject.hh"

#include <glm/vec3.hpp>

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace zenkit {
	class ReadArchive;

	enum class MessageFilterAction : uint32_t {
		NONE = 0,
		TRIGGER = 1,
		UNTRIGGER = 2,
		ENABLE = 3,
		DISABLE = 4,
		TOGGLE = 5,

		// Deprecated entries.
		none ZKREM("renamed to MessageFilterAction::NONE") = NONE,
		trigger ZKREM("renamed to MessageFilterAction::TRIGGER") = TRIGGER,
		untrigger ZKREM("renamed to MessageFilterAction::UNTRIGGER") = UNTRIGGER,
		enable ZKREM("renamed to MessageFilterAction::ENABLE") = ENABLE,
		disable ZKREM("renamed to MessageFilterAction::DISABLE") = DISABLE,
		toggle ZKREM("renamed to MessageFilterAction::TOGGLE") = TOGGLE,
	};

	enum class MoverMessageType : uint32_t {
		FIXED_DIRECT = 0,
		FIXED_ORDER = 1,
		NEXT = 2,
		PREVIOUS = 3,

		// Deprecated entries.
		fixed_direct ZKREM("renamed to MoverMessageType::FIXED_DIRECT") = FIXED_DIRECT,
		fixed_order ZKREM("renamed to MoverMessageType::FIXED_ORDER") = FIXED_ORDER,
		next ZKREM("renamed to MoverMessageType::NEXT") = NEXT,
		previous ZKREM("renamed to MoverMessageType::PREVIOUS") = PREVIOUS,
	};

	enum class TouchCollisionType : std::uint32_t {
		NONE = 0,
		BOX = 1,
		POINT = 2,

		// Deprecated entries.
		none ZKREM("renamed to TouchCollisionType::NONE") = NONE,
		box ZKREM("renamed to TouchCollisionType::BOX") = BOX,
		point ZKREM("renamed to TouchCollisionType::POINT") = POINT,
	};

	/// \brief A VObject used to create animated models.
	///
	/// The visual of such objects can either be a morph mesh or a model with a skeletal animation. The animation
	/// of these objects can be started and stopped using `OnTrigger` and `OnUntrigger` events.
	///
	/// \see https://zk.gothickit.dev/engine/objects/zCVobAnimate/
	struct VAnimate : VirtualObject {
		ZK_OBJECT(ObjectType::zCVobAnimate);

	public:
		/// \brief Whether to start this animation when loading the level.
		/// \see https://zk.gothickit.dev/engine/objects/zCVobAnimate/#startOn
		bool start_on {false};

		// Save-game only variables
		bool s_is_running {false};

		ZKREM("use ::load()") ZKAPI static void parse(VAnimate& obj, ReadArchive& ctx, GameVersion version);

		/// \brief Load this object from the given archive.
		/// \param r The archive to read from;
		/// \param version The version of the game the object was made for.
		ZKAPI void load(ReadArchive& r, GameVersion version) override;

		/// \brief Save this object to the given archive.
		/// \param w The archive to save to.
		/// \param version The version of the game to save for.
		ZKAPI void save(WriteArchive& w, GameVersion version) const override;
	};

	/// \brief Represents an item in the game world.
	///
	/// Items are special VObjects which are tied to a script instance which defines their attributes. They
	/// have physics enabled and can be targeted and picked up by the player.
	///
	/// \see https://zk.gothickit.dev/engine/objects/oCItem/
	struct VItem : VirtualObject {
		ZK_OBJECT(ObjectType::oCItem);

	public:
		/// \brief The name of the script instance representing the item.
		/// \see https://zk.gothickit.dev/engine/objects/oCItem/#itemInstance
		std::string instance;

		// Save-game only variables
		int s_amount;
		int s_flags;

		ZKREM("use ::load()") ZKAPI static void parse(VItem& obj, ReadArchive& ctx, GameVersion version);

		/// \brief Load this object from the given archive.
		/// \param r The archive to read from;
		/// \param version The version of the game the object was made for.
		ZKAPI void load(ReadArchive& r, GameVersion version) override;

		/// \brief Save this object to the given archive.
		/// \param w The archive to save to.
		/// \param version The version of the game to save for.
		ZKAPI void save(WriteArchive& w, GameVersion version) const override;

		[[nodiscard]] ZKAPI uint16_t get_version_identifier(GameVersion game) const override;
	};

	/// \brief Represents a lens flare effect.
	/// \see https://zk.gothickit.dev/engine/objects/zCVobLensFlare/
	struct VLensFlare : VirtualObject {
		ZK_OBJECT(ObjectType::zCVobLensFlare);

	public:
		/// \brief The name of the lens flare effect.
		///
		/// Must be one of the effects listed in `/_work/data/Presets/Lensflare.zen`.
		///
		/// \see https://zk.gothickit.dev/engine/objects/zCVobLensFlare/#lensflareFX
		std::string fx;

		ZKREM("use ::load()") ZKAPI static void parse(VLensFlare& obj, ReadArchive& ctx, GameVersion version);

		/// \brief Load this object from the given archive.
		/// \param r The archive to read from;
		/// \param version The version of the game the object was made for.
		ZKAPI void load(ReadArchive& r, GameVersion version) override;

		/// \brief Save this object to the given archive.
		/// \param w The archive to save to.
		/// \param version The version of the game to save for.
		ZKAPI void save(WriteArchive& w, GameVersion version) const override;

		[[nodiscard]] ZKAPI uint16_t get_version_identifier(GameVersion game) const override;
	};

	/// \brief A particle system in virtual space.
	///
	/// Its effect can be activated and deactivated using the `OnTrigger` and `OnUntrigger` events.
	///
	/// \see https://zk.gothickit.dev/engine/objects/zCPFXController/
	struct VParticleEffectController : VirtualObject {
		ZK_OBJECT(ObjectType::zCPFXController);

	public:
		/// \brief The name of the particle effect.
		///
		/// Corresponds to a `.ZEN` file containing the particle system definition.
		///
		/// \see https://zk.gothickit.dev/engine/objects/zCPFXController/#pfxName
		std::string pfx_name;

		/// \brief Disable the particle effect after it runs once.
		/// \see https://zk.gothickit.dev/engine/objects/zCPFXController/#killVobWhenDone
		bool kill_when_done;

		/// \brief Whether to start the particle effect when the level is loaded.
		/// \see https://zk.gothickit.dev/engine/objects/zCPFXController/#pfxStartOn
		bool initially_running;

		ZKREM("use ::load()")
		ZKAPI static void parse(VParticleEffectController& obj, ReadArchive& ctx, GameVersion version);

		/// \brief Load this object from the given archive.
		/// \param r The archive to read from;
		/// \param version The version of the game the object was made for.
		ZKAPI void load(ReadArchive& r, GameVersion version) override;

		/// \brief Save this object to the given archive.
		/// \param w The archive to save to.
		/// \param version The version of the game to save for.
		ZKAPI void save(WriteArchive& w, GameVersion version) const override;
	};

	/// \brief Transforms any incoming `OnTrigger` and `OnUntrigger` events before passing
	///        them on to the trigger target.
	/// \see https://zk.gothickit.dev/engine/objects/zCMessageFilter/
	struct VMessageFilter : VirtualObject {
		ZK_OBJECT(ObjectType::zCMessageFilter);

	public:
		/// \brief The name of the target VObject.
		///
		/// All `OnTrigger` and `OnUntrigger` messages received by this `zCMessageFilter` VObject are transformed
		/// according to #on_trigger and #on_untrigger and then passed on to the VObject with the name specified in
		/// this field.
		///
		/// \see https://zk.gothickit.dev/engine/objects/zCMessageFilter/#triggerTarget
		std::string target;

		/// \brief The type of event to emit to the #target if this VObject receives an `OnTrigger` message.
		/// \see https://zk.gothickit.dev/engine/objects/zCMessageFilter/#onTrigger
		MessageFilterAction on_trigger;

		/// \brief The type of event to emit to the #target if this VObject receives an `OnUntrigger` message.
		/// \see https://zk.gothickit.dev/engine/objects/zCMessageFilter/#onUntrigger
		MessageFilterAction on_untrigger;

		ZKREM("use ::load()") ZKAPI static void parse(VMessageFilter& obj, ReadArchive& ctx, GameVersion version);

		/// \brief Load this object from the given archive.
		/// \param r The archive to read from;
		/// \param version The version of the game the object was made for.
		ZKAPI void load(ReadArchive& r, GameVersion version) override;

		/// \brief Save this object to the given archive.
		/// \param w The archive to save to.
		/// \param version The version of the game to save for.
		ZKAPI void save(WriteArchive& w, GameVersion version) const override;

		[[nodiscard]] ZKAPI uint16_t get_version_identifier(GameVersion game) const override;
	};

	/// \brief `zCCodeMaster` VObjects keep a list of 'slave' VObjects and keep track of events received by them.
	///
	/// If the master receives an `OnTrigger` event from a slave, it remembers that it did. After it has receives an
	/// `OnTrigger` message from all slaves, it emits an `OnTrigger` event to the #target. Receiving or sending an
	/// `OnTrigger` event is also referred to as an 'activation' for this purpose.
	///
	/// \see https://zk.gothickit.dev/engine/objects/zCCodeMaster/
	struct VCodeMaster : VirtualObject {
		ZK_OBJECT(ObjectType::zCCodeMaster);

	public:
		/// \brief The name of the VObject to send an `OnTrigger` event to after all slaves have
		///        fired in the correct order.
		/// \see https://zk.gothickit.dev/engine/objects/zCCodeMaster/#triggerTarget
		std::string target;

		/// \brief Controls whether the master should keep track of the order it receives messages from its slaves.
		/// \see https://zk.gothickit.dev/engine/objects/zCCodeMaster/#orderRelevant
		bool ordered;

		/// \brief Controls when the slave activation sequence is considered to be incorrect.
		///
		/// If set to `true`, the sequence is considered to be incorrect as soon as a single out-of-order slave is
		/// activated. Otherwise, it is only considered incorrect after all slaves have fired at least once and they
		/// have been activated out-of-order.
		///
		/// \note Only relevant if #ordered is set to `true`.
		/// \see https://zk.gothickit.dev/engine/objects/zCCodeMaster/#firstFalseIsFailure
		bool first_false_is_failure;

		/// \brief The name of the VObject to emit an `OnTrigger` event to if the activation sequence fails.
		/// \note Only relevant if #ordered is set to `true`.
		/// \see https://zk.gothickit.dev/engine/objects/zCCodeMaster/#triggerTargetFailure
		std::string failure_target;

		/// \brief Controls whether slaves can emit `OnUntrigger` events to mark themselves as deactivated.
		/// \note Only relevant if #ordered is set to `false`.
		/// \see https://zk.gothickit.dev/engine/objects/zCCodeMaster/#untriggerCancels
		bool untriggered_cancels;

		/// \brief The names of the slave VObjects.
		/// \see https://zk.gothickit.dev/engine/objects/zCCodeMaster/#slaveVobName
		std::vector<std::string> slaves;

		// Save-game only variables
		uint8_t s_num_triggered_slaves;

		ZKREM("use ::load()") ZKAPI static void parse(VCodeMaster& obj, ReadArchive& ctx, GameVersion version);

		/// \brief Load this object from the given archive.
		/// \param r The archive to read from;
		/// \param version The version of the game the object was made for.
		ZKAPI void load(ReadArchive& r, GameVersion version) override;

		/// \brief Save this object to the given archive.
		/// \param w The archive to save to.
		/// \param version The version of the game to save for.
		ZKAPI void save(WriteArchive& w, GameVersion version) const override;

		[[nodiscard]] ZKAPI uint16_t get_version_identifier(GameVersion game) const override;
	};

	/// \brief A VObject used to control VMover objects of type MoverBehavior::SINGLE_KEYS specifically.
	///
	/// VMoverController objects react to incoming `OnTrigger` events to send a pre-configured event to the target
	/// mover object, controlling its animation. Mover controllers can make movers go to a specific keyframe in
	/// their animation or just step one keyframe back and forth at a time.
	///
	/// \see https://zk.gothickit.dev/engine/objects/zCMoverController/
	struct VMoverController : VirtualObject {
		ZK_OBJECT(ObjectType::zCMoverController);

	public:
		/// \brief The name of the VMover VOject that is controlled by this VMoverController.
		/// \see https://zk.gothickit.dev/engine/objects/zCMoverController/#triggerTarget
		std::string target;

		/// \brief The type of event to emit to the #target when this controller receives an `OnTrigger` event.
		/// \see https://zk.gothickit.dev/engine/objects/zCMoverController/#moverMessage
		MoverMessageType message;

		/// \brief The keyframe of the mover referred to by #target to move to.
		/// \note Only relevant if #message is MoverMessageType::FIXED_DIRECT or MoverMessageType::FIXED_ORDER.
		/// \see https://zk.gothickit.dev/engine/objects/zCMoverController/#gotoFixedKey
		std::int32_t key;

		ZKREM("use ::load()") ZKAPI static void parse(VMoverController& obj, ReadArchive& ctx, GameVersion version);

		/// \brief Load this object from the given archive.
		/// \param r The archive to read from;
		/// \param version The version of the game the object was made for.
		ZKAPI void load(ReadArchive& r, GameVersion version) override;

		/// \brief Save this object to the given archive.
		/// \param w The archive to save to.
		/// \param version The version of the game to save for.
		ZKAPI void save(WriteArchive& w, GameVersion version) const override;

		[[nodiscard]] ZKAPI uint16_t get_version_identifier(GameVersion game) const override;
	};

	/// \brief A VOb which represents a damage source.
	struct VTouchDamage : VirtualObject {
		ZK_OBJECT(ObjectType::oCTouchDamage);

	public:
		float damage;

		bool barrier;
		bool blunt;
		bool edge;
		bool fire;
		bool fly;
		bool magic;
		bool point;
		bool fall;

		float repeat_delay_sec;
		float volume_scale;
		TouchCollisionType collision;

		/// \brief Parses a touch damage VOb the given *ZenGin* archive.
		/// \param[out] obj The object to read.
		/// \param[in,out] ctx The archive reader to read from.
		/// \note After this function returns the position of \p ctx will be at the end of the parsed object.
		/// \throws ParserError if parsing fails.
		/// \see vob::parse
		ZKREM("use ::load()") ZKAPI static void parse(VTouchDamage& obj, ReadArchive& ctx, GameVersion version);

		ZKAPI void load(ReadArchive& r, GameVersion version) override;
		ZKAPI void save(WriteArchive& w, GameVersion version) const override;
		[[nodiscard]] ZKAPI uint16_t get_version_identifier(GameVersion game) const override;
	};

	/// \brief A VOb which represents an earthquake-like effect.
	struct VEarthquake : VirtualObject {
		ZK_OBJECT(ObjectType::zCEarthquake);

	public:
		float radius;
		float duration;
		glm::vec3 amplitude;

		/// \brief Parses an earthquake VOb the given *ZenGin* archive.
		/// \param[out] obj The object to read.
		/// \param[in,out] ctx The archive reader to read from.
		/// \note After this function returns the position of \p ctx will be at the end of the parsed object.
		/// \throws ParserError if parsing fails.
		/// \see vob::parse
		ZKREM("use ::load()") ZKAPI static void parse(VEarthquake& obj, ReadArchive& ctx, GameVersion version);

		ZKAPI void load(ReadArchive& r, GameVersion version) override;
		ZKAPI void save(WriteArchive& w, GameVersion version) const override;
		[[nodiscard]] ZKAPI uint16_t get_version_identifier(GameVersion game) const override;
	};

	struct VNpc : VirtualObject {
		ZK_OBJECT(ObjectType::oCNpc);

	public:
		static std::uint32_t const attribute_count = 8;
		static std::uint32_t const hcs_count = 4;
		static std::uint32_t const missions_count = 5;
		static std::uint32_t const aivar_count = 100;
		static std::uint32_t const packed_count = 9;
		static std::uint32_t const protection_count = 8;

		struct Talent : Object {
			static constexpr ObjectType TYPE = ObjectType::oCNpcTalent;

			int talent;
			int value;
			int skill;

			[[nodiscard]] ObjectType get_object_type() const override {
				return TYPE;
			}

			void load(ReadArchive& r, GameVersion version) override;
		};

		using talent ZKREM("renamed to Talent") = Talent;

		struct Slot {
			bool used;
			std::string name;
			std::shared_ptr<VItem> item {};
			bool in_inventory;
		};

		using slot ZKREM("renamed to Slot") = Slot;

		std::string npc_instance;
		glm::vec3 model_scale;
		float model_fatness;

		std::vector<std::string> overlays;

		int flags;
		int guild;
		int guild_true;
		int level;
		int xp;
		int xp_next_level;
		int lp;

		std::vector<std::shared_ptr<Talent>> talents;

		int fight_tactic;
		int fight_mode;
		bool wounded;
		bool mad;
		int mad_time;
		bool player;

		int attributes[attribute_count];
		int hcs[hcs_count];
		int missions[missions_count];

		std::string start_ai_state;
		int aivar[aivar_count];
		std::string script_waypoint;
		int attitude;
		int attitude_temp;
		int name_nr;
		bool move_lock;

		std::string packed[packed_count];
		std::vector<std::shared_ptr<VItem>> items;
		std::vector<Slot> slots;

		bool current_state_valid;
		std::string current_state_name;
		int current_state_index;
		bool current_state_is_routine;

		bool next_state_valid;
		std::string next_state_name;
		int next_state_index;
		bool next_state_is_routine;

		int last_ai_state;
		bool has_routine;
		bool routine_changed;
		bool routine_overlay;
		int routine_overlay_count;
		int walkmode_routine;
		bool weaponmode_routine;
		bool start_new_routine;
		int ai_state_driven;
		glm::vec3 ai_state_pos;
		std::string current_routine;
		bool respawn;
		int respawn_time;

		int protection[protection_count];

		int bs_interruptable_override {0};
		int npc_type {0};
		int spell_mana {0};

		/// \brief Parses an NPC VOb from the given *ZenGin* archive.
		/// \param[out] obj The object to read.
		/// \param[in,out] ctx The archive reader to read from.
		/// \note After this function returns the position of \p ctx will be at the end of the parsed object.
		/// \throws ParserError if parsing fails.
		/// \see vob::parse
		ZKREM("use ::load()") ZKAPI static void parse(VNpc& obj, ReadArchive& ctx, GameVersion version);

		ZKAPI void load(ReadArchive& r, GameVersion version) override;
	};

	struct VScreenEffect : VirtualObject {
		ZK_OBJECT(ObjectType::zCVobScreenFX)

	public:
		ZKAPI void load(ReadArchive& r, GameVersion version) override;
		ZKAPI void save(WriteArchive& w, GameVersion version) const override;
	};
} // namespace zenkit
