# Copyright 2018 Google Inc.  All rights reserved
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

workspace(name = "com_google_absl_hello_world")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
# abseil
http_archive(
    name = "absl",
    strip_prefix = "abseil-cpp-master",
    urls = ["https://github.com/abseil/abseil-cpp/archive/master.zip"],
)

http_archive(
    name = "com_github_grpc_grpc",
    strip_prefix = "grpc-109c570727c3089fef655edcdd0dd02cc5958010",
    urls = [
        "https://github.com/grpc/grpc/archive/109c570727c3089fef655edcdd0dd02cc5958010.tar.gz",
    ],
)

# Google Test
http_archive(
    name = "gtest",
    strip_prefix = "googletest-master",
    urls = ["https://github.com/google/googletest/archive/master.zip"],
)

load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")

grpc_deps()
