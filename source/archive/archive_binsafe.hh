// Copyright © 2022 Luis Michaelis <lmichaelis.all+dev@gmail.com>
// SPDX-License-Identifier: MIT
#pragma once
#include <phoenix/archive.hh>
#include <unordered_map>
#include <vector>

namespace phoenix {
	constexpr std::uint8_t type_sizes[] = {
	    0,                        // ?            = 0x00
	    0,                        // bs_string    = 0x01,
	    sizeof(std::int32_t),     // bs_int       = 0x02,
	    sizeof(float),            // bs_float     = 0x03,
	    sizeof(std::uint8_t),     // bs_byte      = 0x04,
	    sizeof(std::uint16_t),    // bs_word      = 0x05,
	    sizeof(std::uint32_t),    // bs_bool      = 0x06,
	    sizeof(float) * 3,        // bs_vec3      = 0x07,
	    sizeof(std::uint8_t) * 4, // bs_color     = 0x08,
	    0,                        // bs_raw       = 0x09,
	    0,                        // ?            = 0x0A
	    0,                        // ?            = 0x0B
	    0,                        // ?            = 0x0C
	    0,                        // ?            = 0x0D
	    0,                        // ?            = 0x0E
	    0,                        // ?            = 0x0F
	    0,                        // bs_raw_float = 0x10,
	    sizeof(std::uint32_t),    // bs_enum      = 0x11,
	    sizeof(std::uint32_t),    // bs_hash      = 0x12,
	};

	enum archive_binsafe_type {
		bs_string = 0x1,
		bs_int = 0x2,
		bs_float = 0x3,
		bs_byte = 0x4,
		bs_word = 0x5,
		bs_bool = 0x6,
		bs_vec3 = 0x7,
		bs_color = 0x8,
		bs_raw = 0x9,
		bs_raw_float = 0x10,
		bs_enum = 0x11,
		bs_hash = 0x12,
	};

	struct hash_table_entry {
		std::string key;
		std::uint32_t hash;
	};

	class archive_reader_binsafe final : public archive_reader {
	public:
		inline archive_reader_binsafe(buffer& in, archive_header&& header) : archive_reader(in, std::move(header)) {}

		bool read_object_begin(archive_object& obj) override;
		bool read_object_end() override;
		std::string read_string() override;
		std::int32_t read_int() override;
		float read_float() override;
		std::uint8_t read_byte() override;
		std::uint16_t read_word() override;
		std::uint32_t read_enum() override;
		bool read_bool() override;
		glm::u8vec4 read_color() override;
		glm::vec3 read_vec3() override;
		glm::vec2 read_vec2() override;
		bounding_box read_bbox() override;
		glm::mat3x3 read_mat3x3() override;
		buffer read_raw_bytes() override;

	protected:
		void skip_entry() override;

	protected:
		void read_header() override;

		void skip_optional_hash();
		std::uint16_t assure_entry(archive_binsafe_type tp);

	private:
		std::uint32_t _m_object_count {0};
		std::uint32_t _m_bs_version {0};

		std::vector<hash_table_entry> _m_hash_table_entries;
	};
} // namespace phoenix
