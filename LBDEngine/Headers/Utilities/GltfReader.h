// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include "GLTFSDK/GLTF.h"
#include "GLTFSDK/GLTFResourceReader.h"
#include "GLTFSDK/GLBResourceReader.h"
#include "GLTFSDK/Deserialize.h"

// Replace this with <filesystem> (and use std::filesystem rather than
// std::experimental::filesystem) if your toolchain fully supports C++17
#include <experimental/filesystem>

#include <fstream>
#include <sstream>
#include <iostream>

#include <cassert>
#include <cstdlib>
