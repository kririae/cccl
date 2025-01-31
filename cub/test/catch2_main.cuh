/******************************************************************************
* Copyright (c) 2011-2022, NVIDIA CORPORATION.  All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the NVIDIA CORPORATION nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL NVIDIA CORPORATION BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************/

#pragma once

#include <cuda_runtime.h>
#include <iostream>

#ifdef CUB_CONFIG_MAIN
#define CATCH_CONFIG_RUNNER
#endif

#include <catch2/catch.hpp>

#ifdef CUB_CONFIG_MAIN
#include <cuda_runtime.h>

static int device_guard(int device_id)
{
  int device_count {};
  if (cudaGetDeviceCount(&device_count) != cudaSuccess)
  {
    std::cerr << "Can't query devices number." << std::endl;
    std::exit(-1);
  }

  if (device_id >= device_count || device_id < 0)
  {
    std::cerr << "Invalid device ID: " << device_id << std::endl;
    std::exit(-1);
  }

  return device_id;
}


int main(int argc, char *argv[])
{
  Catch::Session session;

  int device_id {};

  // Build a new parser on top of Catch's
  using namespace Catch::clara;
  auto cli = session.cli()
           | Opt(device_id, "device")["-d"]["--device"]("device id to use");
  session.cli(cli);

  int returnCode = session.applyCommandLine(argc, argv);
  if(returnCode != 0)
  {
    return returnCode;
  }

  cudaSetDevice(device_guard(device_id));

  return session.run(argc, argv);
}
#endif
