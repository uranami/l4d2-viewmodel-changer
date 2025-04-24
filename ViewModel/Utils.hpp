#pragma once
#include <Windows.h>

typedef void* (*CreateInterfaceFn)(const char* name, int* returnCode);

std::uintptr_t FindPattern(std::string_view module_name, std::string_view pattern)
{
	const auto module_handle = GetModuleHandleA(module_name.data());
	if (!module_handle)
		return 0U;

	const auto module_address = reinterpret_cast<std::uint8_t*>(module_handle);
	const auto module_dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module_handle);
	const auto module_nt_header = reinterpret_cast<PIMAGE_NT_HEADERS>(module_address + module_dos_header->e_lfanew);
	const auto image_size = module_nt_header->OptionalHeader.SizeOfImage;

	static auto pattern_to_byte = [](std::string_view pattern)
		{
			/// Prerequisites
			auto bytes = std::vector<int>{};
			auto start = const_cast<char*>(pattern.data());
			auto end = const_cast<char*>(pattern.data()) + strlen(pattern.data());

			/// Convert signature into corresponding bytes
			for (auto current = start; current < end; ++current)
			{
				/// Is current byte a wildcard? Simply ignore that that byte later
				if (*current == '?')
				{
					++current;

					/// Check if following byte is also a wildcard
					if (*current == '?')
						++current;

					/// Dummy byte
					bytes.push_back(-1);
				}
				else
				{
					/// Convert character to byte on hexadecimal base
					bytes.push_back(strtoul(current, &current, 16));
				}
			}
			return bytes;
		};

	auto bytes = pattern_to_byte(pattern);
	auto bytes_size = bytes.size();

	for (auto i = 0UL; i < image_size - bytes_size; ++i)
	{
		auto found = true;

		for (auto j = 0UL; j < bytes_size; ++j)
		{
			if (module_address[i + j] != bytes[j] && bytes[j] != -1)
			{
				found = false;
				break;
			}
		}

		if (found)
			return reinterpret_cast<std::uintptr_t>(&module_address[i]);
	}
	return 0U;
}

