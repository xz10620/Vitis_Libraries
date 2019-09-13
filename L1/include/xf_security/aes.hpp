/*
 * Copyright 2019 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file aes.hpp
 * @brief header file for Advanced Encryption Standard relate function.
 * This file part of XF Security Library.
 *
 * @detail Currently we have Aes256_Encryption for AES256 standard.
 */

#ifndef _XF_SECURITY_AES_HPP_
#define _XF_SECURITY_AES_HPP_

#include <ap_int.h>
#include <hls_stream.h>
#ifndef __SYNTHESIS__
#include <iostream>
#endif
namespace xf {
namespace security {

const ap_uint<8> sbox_Rcon[2048] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x1,  0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, 0xca, 0x82, 0xc9,
    0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f,
    0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, 0x4,  0xc7, 0x23, 0xc3, 0x18, 0x96, 0x5,  0x9a, 0x7,
    0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3,
    0x29, 0xe3, 0x2f, 0x84, 0x53, 0xd1, 0x0,  0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58,
    0xcf, 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x2,  0x7f, 0x50, 0x3c, 0x9f, 0xa8, 0x51, 0xa3,
    0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, 0xcd, 0x0c, 0x13, 0xec, 0x5f,
    0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
    0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x6,  0x24, 0x5c, 0xc2, 0xd3, 0xac,
    0x62, 0x91, 0x95, 0xe4, 0x79, 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a,
    0xae, 0x08, 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, 0x70,
    0x3e, 0xb5, 0x66, 0x48, 0x3,  0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, 0xe1, 0xf8, 0x98, 0x11,
    0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, 0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42,
    0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16, 0x62, 0x7d, 0x76, 0x7a, 0xf3, 0x6a, 0x6e, 0xc4, 0x31, 0x0,
    0x66, 0x2a, 0xff, 0xd6, 0xaa, 0x77, 0xcb, 0x83, 0xc8, 0x7c, 0xfb, 0x58, 0x46, 0xf1, 0xac, 0xd5, 0xa3, 0xae, 0x9d,
    0xa5, 0x73, 0xc1, 0xb6, 0xfc, 0x92, 0x27, 0x37, 0x3e, 0xf6, 0xcd, 0x35, 0xa4, 0xe4, 0xf0, 0x70, 0xd9, 0x30, 0x14,
    0x5,  0xc6, 0x22, 0xc2, 0x19, 0x97, 0x4,  0x9b, 0x6,  0x13, 0x81, 0xe3, 0xea, 0x26, 0xb3, 0x74, 0x08, 0x82, 0x2d,
    0x1b, 0x1a, 0x6f, 0x5b, 0xa1, 0x53, 0x3a, 0xd7, 0xb2, 0x28, 0xe2, 0x2e, 0x85, 0x52, 0xd0, 0x1,  0xec, 0x21, 0xfd,
    0xb0, 0x5a, 0x6b, 0xca, 0xbf, 0x38, 0x4b, 0x4d, 0x59, 0xce, 0xd1, 0xee, 0xab, 0xfa, 0x42, 0x4c, 0x32, 0x84, 0x44,
    0xf8, 0x3,  0x7e, 0x51, 0x3d, 0x9e, 0xa9, 0x50, 0xa2, 0x41, 0x8e, 0x93, 0x9c, 0x39, 0xf4, 0xbd, 0xb7, 0xdb, 0x20,
    0x11, 0xfe, 0xf2, 0xd3, 0xcc, 0x0d, 0x12, 0xed, 0x5e, 0x96, 0x45, 0x16, 0xc5, 0xa6, 0x7f, 0x3c, 0x65, 0x5c, 0x18,
    0x72, 0x61, 0x80, 0x4e, 0xdd, 0x23, 0x2b, 0x91, 0x89, 0x47, 0xef, 0xb9, 0x15, 0xdf, 0x5f, 0x0a, 0xda, 0xe1, 0x33,
    0x3b, 0x0b, 0x48, 0x7,  0x25, 0x5d, 0xc3, 0xd2, 0xad, 0x63, 0x90, 0x94, 0xe5, 0x78, 0xe6, 0xc9, 0x36, 0x6c, 0x8c,
    0xd4, 0x4f, 0xa8, 0x6d, 0x57, 0xf5, 0xeb, 0x64, 0x7b, 0xaf, 0x09, 0xbb, 0x79, 0x24, 0x2f, 0x1d, 0xa7, 0xb5, 0xc7,
    0xe9, 0xdc, 0x75, 0x1e, 0x4a, 0xbc, 0x8a, 0x8b, 0x71, 0x3f, 0xb4, 0x67, 0x49, 0x2,  0xf7, 0x0f, 0x60, 0x34, 0x56,
    0xb8, 0x87, 0xc0, 0x1c, 0x9f, 0xe0, 0xf9, 0x99, 0x10, 0x68, 0xd8, 0x8f, 0x95, 0x9a, 0x1f, 0x86, 0xe8, 0xcf, 0x54,
    0x29, 0xde, 0x8d, 0xa0, 0x88, 0x0c, 0xbe, 0xe7, 0x43, 0x69, 0x40, 0x98, 0x2c, 0x0e, 0xb1, 0x55, 0xba, 0x17, 0x61,
    0x7e, 0x75, 0x79, 0xf0, 0x69, 0x6d, 0xc7, 0x32, 0x3,  0x65, 0x29, 0xfc, 0xd5, 0xa9, 0x74, 0xc8, 0x80, 0xcb, 0x7f,
    0xf8, 0x5b, 0x45, 0xf2, 0xaf, 0xd6, 0xa0, 0xad, 0x9e, 0xa6, 0x70, 0xc2, 0xb5, 0xff, 0x91, 0x24, 0x34, 0x3d, 0xf5,
    0xce, 0x36, 0xa7, 0xe7, 0xf3, 0x73, 0xda, 0x33, 0x17, 0x6,  0xc5, 0x21, 0xc1, 0x1a, 0x94, 0x7,  0x98, 0x5,  0x10,
    0x82, 0xe0, 0xe9, 0x25, 0xb0, 0x77, 0x0b, 0x81, 0x2e, 0x18, 0x19, 0x6c, 0x58, 0xa2, 0x50, 0x39, 0xd4, 0xb1, 0x2b,
    0xe1, 0x2d, 0x86, 0x51, 0xd3, 0x2,  0xef, 0x22, 0xfe, 0xb3, 0x59, 0x68, 0xc9, 0xbc, 0x3b, 0x48, 0x4e, 0x5a, 0xcd,
    0xd2, 0xed, 0xa8, 0xf9, 0x41, 0x4f, 0x31, 0x87, 0x47, 0xfb, 0x0,  0x7d, 0x52, 0x3e, 0x9d, 0xaa, 0x53, 0xa1, 0x42,
    0x8d, 0x90, 0x9f, 0x3a, 0xf7, 0xbe, 0xb4, 0xd8, 0x23, 0x12, 0xfd, 0xf1, 0xd0, 0xcf, 0x0e, 0x11, 0xee, 0x5d, 0x95,
    0x46, 0x15, 0xc6, 0xa5, 0x7c, 0x3f, 0x66, 0x5f, 0x1b, 0x71, 0x62, 0x83, 0x4d, 0xde, 0x20, 0x28, 0x92, 0x8a, 0x44,
    0xec, 0xba, 0x16, 0xdc, 0x5c, 0x09, 0xd9, 0xe2, 0x30, 0x38, 0x08, 0x4b, 0x4,  0x26, 0x5e, 0xc0, 0xd1, 0xae, 0x60,
    0x93, 0x97, 0xe6, 0x7b, 0xe5, 0xca, 0x35, 0x6f, 0x8f, 0xd7, 0x4c, 0xab, 0x6e, 0x54, 0xf6, 0xe8, 0x67, 0x78, 0xac,
    0x0a, 0xb8, 0x7a, 0x27, 0x2c, 0x1e, 0xa4, 0xb6, 0xc4, 0xea, 0xdf, 0x76, 0x1d, 0x49, 0xbf, 0x89, 0x88, 0x72, 0x3c,
    0xb7, 0x64, 0x4a, 0x1,  0xf4, 0x0c, 0x63, 0x37, 0x55, 0xbb, 0x84, 0xc3, 0x1f, 0x9c, 0xe3, 0xfa, 0x9a, 0x13, 0x6b,
    0xdb, 0x8c, 0x96, 0x99, 0x1c, 0x85, 0xeb, 0xcc, 0x57, 0x2a, 0xdd, 0x8e, 0xa3, 0x8b, 0x0f, 0xbd, 0xe4, 0x40, 0x6a,
    0x43, 0x9b, 0x2f, 0x0d, 0xb2, 0x56, 0xb9, 0x14, 0x67, 0x78, 0x73, 0x7f, 0xf6, 0x6f, 0x6b, 0xc1, 0x34, 0x5,  0x63,
    0x2f, 0xfa, 0xd3, 0xaf, 0x72, 0xce, 0x86, 0xcd, 0x79, 0xfe, 0x5d, 0x43, 0xf4, 0xa9, 0xd0, 0xa6, 0xab, 0x98, 0xa0,
    0x76, 0xc4, 0xb3, 0xf9, 0x97, 0x22, 0x32, 0x3b, 0xf3, 0xc8, 0x30, 0xa1, 0xe1, 0xf5, 0x75, 0xdc, 0x35, 0x11, 0x0,
    0xc3, 0x27, 0xc7, 0x1c, 0x92, 0x1,  0x9e, 0x3,  0x16, 0x84, 0xe6, 0xef, 0x23, 0xb6, 0x71, 0x0d, 0x87, 0x28, 0x1e,
    0x1f, 0x6a, 0x5e, 0xa4, 0x56, 0x3f, 0xd2, 0xb7, 0x2d, 0xe7, 0x2b, 0x80, 0x57, 0xd5, 0x4,  0xe9, 0x24, 0xf8, 0xb5,
    0x5f, 0x6e, 0xcf, 0xba, 0x3d, 0x4e, 0x48, 0x5c, 0xcb, 0xd4, 0xeb, 0xae, 0xff, 0x47, 0x49, 0x37, 0x81, 0x41, 0xfd,
    0x6,  0x7b, 0x54, 0x38, 0x9b, 0xac, 0x55, 0xa7, 0x44, 0x8b, 0x96, 0x99, 0x3c, 0xf1, 0xb8, 0xb2, 0xde, 0x25, 0x14,
    0xfb, 0xf7, 0xd6, 0xc9, 0x08, 0x17, 0xe8, 0x5b, 0x93, 0x40, 0x13, 0xc0, 0xa3, 0x7a, 0x39, 0x60, 0x59, 0x1d, 0x77,
    0x64, 0x85, 0x4b, 0xd8, 0x26, 0x2e, 0x94, 0x8c, 0x42, 0xea, 0xbc, 0x10, 0xda, 0x5a, 0x0f, 0xdf, 0xe4, 0x36, 0x3e,
    0x0e, 0x4d, 0x2,  0x20, 0x58, 0xc6, 0xd7, 0xa8, 0x66, 0x95, 0x91, 0xe0, 0x7d, 0xe3, 0xcc, 0x33, 0x69, 0x89, 0xd1,
    0x4a, 0xad, 0x68, 0x52, 0xf0, 0xee, 0x61, 0x7e, 0xaa, 0x0c, 0xbe, 0x7c, 0x21, 0x2a, 0x18, 0xa2, 0xb0, 0xc2, 0xec,
    0xd9, 0x70, 0x1b, 0x4f, 0xb9, 0x8f, 0x8e, 0x74, 0x3a, 0xb1, 0x62, 0x4c, 0x7,  0xf2, 0x0a, 0x65, 0x31, 0x53, 0xbd,
    0x82, 0xc5, 0x19, 0x9a, 0xe5, 0xfc, 0x9c, 0x15, 0x6d, 0xdd, 0x8a, 0x90, 0x9f, 0x1a, 0x83, 0xed, 0xca, 0x51, 0x2c,
    0xdb, 0x88, 0xa5, 0x8d, 0x09, 0xbb, 0xe2, 0x46, 0x6c, 0x45, 0x9d, 0x29, 0x0b, 0xb4, 0x50, 0xbf, 0x12, 0x6b, 0x74,
    0x7f, 0x73, 0xfa, 0x63, 0x67, 0xcd, 0x38, 0x09, 0x6f, 0x23, 0xf6, 0xdf, 0xa3, 0x7e, 0xc2, 0x8a, 0xc1, 0x75, 0xf2,
    0x51, 0x4f, 0xf8, 0xa5, 0xdc, 0xaa, 0xa7, 0x94, 0xac, 0x7a, 0xc8, 0xbf, 0xf5, 0x9b, 0x2e, 0x3e, 0x37, 0xff, 0xc4,
    0x3c, 0xad, 0xed, 0xf9, 0x79, 0xd0, 0x39, 0x1d, 0x0c, 0xcf, 0x2b, 0xcb, 0x10, 0x9e, 0x0d, 0x92, 0x0f, 0x1a, 0x88,
    0xea, 0xe3, 0x2f, 0xba, 0x7d, 0x1,  0x8b, 0x24, 0x12, 0x13, 0x66, 0x52, 0xa8, 0x5a, 0x33, 0xde, 0xbb, 0x21, 0xeb,
    0x27, 0x8c, 0x5b, 0xd9, 0x08, 0xe5, 0x28, 0xf4, 0xb9, 0x53, 0x62, 0xc3, 0xb6, 0x31, 0x42, 0x44, 0x50, 0xc7, 0xd8,
    0xe7, 0xa2, 0xf3, 0x4b, 0x45, 0x3b, 0x8d, 0x4d, 0xf1, 0x0a, 0x77, 0x58, 0x34, 0x97, 0xa0, 0x59, 0xab, 0x48, 0x87,
    0x9a, 0x95, 0x30, 0xfd, 0xb4, 0xbe, 0xd2, 0x29, 0x18, 0xf7, 0xfb, 0xda, 0xc5, 0x4,  0x1b, 0xe4, 0x57, 0x9f, 0x4c,
    0x1f, 0xcc, 0xaf, 0x76, 0x35, 0x6c, 0x55, 0x11, 0x7b, 0x68, 0x89, 0x47, 0xd4, 0x2a, 0x22, 0x98, 0x80, 0x4e, 0xe6,
    0xb0, 0x1c, 0xd6, 0x56, 0x3,  0xd3, 0xe8, 0x3a, 0x32, 0x2,  0x41, 0x0e, 0x2c, 0x54, 0xca, 0xdb, 0xa4, 0x6a, 0x99,
    0x9d, 0xec, 0x71, 0xef, 0xc0, 0x3f, 0x65, 0x85, 0xdd, 0x46, 0xa1, 0x64, 0x5e, 0xfc, 0xe2, 0x6d, 0x72, 0xa6, 0x0,
    0xb2, 0x70, 0x2d, 0x26, 0x14, 0xae, 0xbc, 0xce, 0xe0, 0xd5, 0x7c, 0x17, 0x43, 0xb5, 0x83, 0x82, 0x78, 0x36, 0xbd,
    0x6e, 0x40, 0x0b, 0xfe, 0x6,  0x69, 0x3d, 0x5f, 0xb1, 0x8e, 0xc9, 0x15, 0x96, 0xe9, 0xf0, 0x90, 0x19, 0x61, 0xd1,
    0x86, 0x9c, 0x93, 0x16, 0x8f, 0xe1, 0xc6, 0x5d, 0x20, 0xd7, 0x84, 0xa9, 0x81, 0x5,  0xb7, 0xee, 0x4a, 0x60, 0x49,
    0x91, 0x25, 0x7,  0xb8, 0x5c, 0xb3, 0x1e, 0x73, 0x6c, 0x67, 0x6b, 0xe2, 0x7b, 0x7f, 0xd5, 0x20, 0x11, 0x77, 0x3b,
    0xee, 0xc7, 0xbb, 0x66, 0xda, 0x92, 0xd9, 0x6d, 0xea, 0x49, 0x57, 0xe0, 0xbd, 0xc4, 0xb2, 0xbf, 0x8c, 0xb4, 0x62,
    0xd0, 0xa7, 0xed, 0x83, 0x36, 0x26, 0x2f, 0xe7, 0xdc, 0x24, 0xb5, 0xf5, 0xe1, 0x61, 0xc8, 0x21, 0x5,  0x14, 0xd7,
    0x33, 0xd3, 0x08, 0x86, 0x15, 0x8a, 0x17, 0x2,  0x90, 0xf2, 0xfb, 0x37, 0xa2, 0x65, 0x19, 0x93, 0x3c, 0x0a, 0x0b,
    0x7e, 0x4a, 0xb0, 0x42, 0x2b, 0xc6, 0xa3, 0x39, 0xf3, 0x3f, 0x94, 0x43, 0xc1, 0x10, 0xfd, 0x30, 0xec, 0xa1, 0x4b,
    0x7a, 0xdb, 0xae, 0x29, 0x5a, 0x5c, 0x48, 0xdf, 0xc0, 0xff, 0xba, 0xeb, 0x53, 0x5d, 0x23, 0x95, 0x55, 0xe9, 0x12,
    0x6f, 0x40, 0x2c, 0x8f, 0xb8, 0x41, 0xb3, 0x50, 0x9f, 0x82, 0x8d, 0x28, 0xe5, 0xac, 0xa6, 0xca, 0x31, 0x0,  0xef,
    0xe3, 0xc2, 0xdd, 0x1c, 0x3,  0xfc, 0x4f, 0x87, 0x54, 0x7,  0xd4, 0xb7, 0x6e, 0x2d, 0x74, 0x4d, 0x09, 0x63, 0x70,
    0x91, 0x5f, 0xcc, 0x32, 0x3a, 0x80, 0x98, 0x56, 0xfe, 0xa8, 0x4,  0xce, 0x4e, 0x1b, 0xcb, 0xf0, 0x22, 0x2a, 0x1a,
    0x59, 0x16, 0x34, 0x4c, 0xd2, 0xc3, 0xbc, 0x72, 0x81, 0x85, 0xf4, 0x69, 0xf7, 0xd8, 0x27, 0x7d, 0x9d, 0xc5, 0x5e,
    0xb9, 0x7c, 0x46, 0xe4, 0xfa, 0x75, 0x6a, 0xbe, 0x18, 0xaa, 0x68, 0x35, 0x3e, 0x0c, 0xb6, 0xa4, 0xd6, 0xf8, 0xcd,
    0x64, 0x0f, 0x5b, 0xad, 0x9b, 0x9a, 0x60, 0x2e, 0xa5, 0x76, 0x58, 0x13, 0xe6, 0x1e, 0x71, 0x25, 0x47, 0xa9, 0x96,
    0xd1, 0x0d, 0x8e, 0xf1, 0xe8, 0x88, 0x1,  0x79, 0xc9, 0x9e, 0x84, 0x8b, 0x0e, 0x97, 0xf9, 0xde, 0x45, 0x38, 0xcf,
    0x9c, 0xb1, 0x99, 0x1d, 0xaf, 0xf6, 0x52, 0x78, 0x51, 0x89, 0x3d, 0x1f, 0xa0, 0x44, 0xab, 0x6,  0x43, 0x5c, 0x57,
    0x5b, 0xd2, 0x4b, 0x4f, 0xe5, 0x10, 0x21, 0x47, 0x0b, 0xde, 0xf7, 0x8b, 0x56, 0xea, 0xa2, 0xe9, 0x5d, 0xda, 0x79,
    0x67, 0xd0, 0x8d, 0xf4, 0x82, 0x8f, 0xbc, 0x84, 0x52, 0xe0, 0x97, 0xdd, 0xb3, 0x6,  0x16, 0x1f, 0xd7, 0xec, 0x14,
    0x85, 0xc5, 0xd1, 0x51, 0xf8, 0x11, 0x35, 0x24, 0xe7, 0x3,  0xe3, 0x38, 0xb6, 0x25, 0xba, 0x27, 0x32, 0xa0, 0xc2,
    0xcb, 0x7,  0x92, 0x55, 0x29, 0xa3, 0x0c, 0x3a, 0x3b, 0x4e, 0x7a, 0x80, 0x72, 0x1b, 0xf6, 0x93, 0x09, 0xc3, 0x0f,
    0xa4, 0x73, 0xf1, 0x20, 0xcd, 0x0,  0xdc, 0x91, 0x7b, 0x4a, 0xeb, 0x9e, 0x19, 0x6a, 0x6c, 0x78, 0xef, 0xf0, 0xcf,
    0x8a, 0xdb, 0x63, 0x6d, 0x13, 0xa5, 0x65, 0xd9, 0x22, 0x5f, 0x70, 0x1c, 0xbf, 0x88, 0x71, 0x83, 0x60, 0xaf, 0xb2,
    0xbd, 0x18, 0xd5, 0x9c, 0x96, 0xfa, 0x1,  0x30, 0xdf, 0xd3, 0xf2, 0xed, 0x2c, 0x33, 0xcc, 0x7f, 0xb7, 0x64, 0x37,
    0xe4, 0x87, 0x5e, 0x1d, 0x44, 0x7d, 0x39, 0x53, 0x40, 0xa1, 0x6f, 0xfc, 0x2,  0x0a, 0xb0, 0xa8, 0x66, 0xce, 0x98,
    0x34, 0xfe, 0x7e, 0x2b, 0xfb, 0xc0, 0x12, 0x1a, 0x2a, 0x69, 0x26, 0x4,  0x7c, 0xe2, 0xf3, 0x8c, 0x42, 0xb1, 0xb5,
    0xc4, 0x59, 0xc7, 0xe8, 0x17, 0x4d, 0xad, 0xf5, 0x6e, 0x89, 0x4c, 0x76, 0xd4, 0xca, 0x45, 0x5a, 0x8e, 0x28, 0x9a,
    0x58, 0x5,  0x0e, 0x3c, 0x86, 0x94, 0xe6, 0xc8, 0xfd, 0x54, 0x3f, 0x6b, 0x9d, 0xab, 0xaa, 0x50, 0x1e, 0x95, 0x46,
    0x68, 0x23, 0xd6, 0x2e, 0x41, 0x15, 0x77, 0x99, 0xa6, 0xe1, 0x3d, 0xbe, 0xc1, 0xd8, 0xb8, 0x31, 0x49, 0xf9, 0xae,
    0xb4, 0xbb, 0x3e, 0xa7, 0xc9, 0xee, 0x75, 0x08, 0xff, 0xac, 0x81, 0xa9, 0x2d, 0x9f, 0xc6, 0x62, 0x48, 0x61, 0xb9,
    0x0d, 0x2f, 0x90, 0x74, 0x9b, 0x36, 0x23, 0x3c, 0x37, 0x3b, 0xb2, 0x2b, 0x2f, 0x85, 0x70, 0x41, 0x27, 0x6b, 0xbe,
    0x97, 0xeb, 0x36, 0x8a, 0xc2, 0x89, 0x3d, 0xba, 0x19, 0x7,  0xb0, 0xed, 0x94, 0xe2, 0xef, 0xdc, 0xe4, 0x32, 0x80,
    0xf7, 0xbd, 0xd3, 0x66, 0x76, 0x7f, 0xb7, 0x8c, 0x74, 0xe5, 0xa5, 0xb1, 0x31, 0x98, 0x71, 0x55, 0x44, 0x87, 0x63,
    0x83, 0x58, 0xd6, 0x45, 0xda, 0x47, 0x52, 0xc0, 0xa2, 0xab, 0x67, 0xf2, 0x35, 0x49, 0xc3, 0x6c, 0x5a, 0x5b, 0x2e,
    0x1a, 0xe0, 0x12, 0x7b, 0x96, 0xf3, 0x69, 0xa3, 0x6f, 0xc4, 0x13, 0x91, 0x40, 0xad, 0x60, 0xbc, 0xf1, 0x1b, 0x2a,
    0x8b, 0xfe, 0x79, 0x0a, 0x0c, 0x18, 0x8f, 0x90, 0xaf, 0xea, 0xbb, 0x3,  0x0d, 0x73, 0xc5, 0x5,  0xb9, 0x42, 0x3f,
    0x10, 0x7c, 0xdf, 0xe8, 0x11, 0xe3, 0x0,  0xcf, 0xd2, 0xdd, 0x78, 0xb5, 0xfc, 0xf6, 0x9a, 0x61, 0x50, 0xbf, 0xb3,
    0x92, 0x8d, 0x4c, 0x53, 0xac, 0x1f, 0xd7, 0x4,  0x57, 0x84, 0xe7, 0x3e, 0x7d, 0x24, 0x1d, 0x59, 0x33, 0x20, 0xc1,
    0x0f, 0x9c, 0x62, 0x6a, 0xd0, 0xc8, 0x6,  0xae, 0xf8, 0x54, 0x9e, 0x1e, 0x4b, 0x9b, 0xa0, 0x72, 0x7a, 0x4a, 0x09,
    0x46, 0x64, 0x1c, 0x82, 0x93, 0xec, 0x22, 0xd1, 0xd5, 0xa4, 0x39, 0xa7, 0x88, 0x77, 0x2d, 0xcd, 0x95, 0x0e, 0xe9,
    0x2c, 0x16, 0xb4, 0xaa, 0x25, 0x3a, 0xee, 0x48, 0xfa, 0x38, 0x65, 0x6e, 0x5c, 0xe6, 0xf4, 0x86, 0xa8, 0x9d, 0x34,
    0x5f, 0x0b, 0xfd, 0xcb, 0xca, 0x30, 0x7e, 0xf5, 0x26, 0x08, 0x43, 0xb6, 0x4e, 0x21, 0x75, 0x17, 0xf9, 0xc6, 0x81,
    0x5d, 0xde, 0xa1, 0xb8, 0xd8, 0x51, 0x29, 0x99, 0xce, 0xd4, 0xdb, 0x5e, 0xc7, 0xa9, 0x8e, 0x15, 0x68, 0x9f, 0xcc,
    0xe1, 0xc9, 0x4d, 0xff, 0xa6, 0x2,  0x28, 0x1,  0xd9, 0x6d, 0x4f, 0xf0, 0x14, 0xfb, 0x56};

const ap_uint<16> sbox_mix_col_1[256] = {
    0xc663, 0xf87c, 0xee77, 0xf67b, 0xfff2, 0xd66b, 0xde6f, 0x91c5, 0x6030, 0x201,  0xce67, 0x562b, 0xe7fe, 0xb5d7,
    0x4dab, 0xec76, 0x8fca, 0x1f82, 0x89c9, 0xfa7d, 0xeffa, 0xb259, 0x8e47, 0xfbf0, 0x41ad, 0xb3d4, 0x5fa2, 0x45af,
    0x239c, 0x53a4, 0xe472, 0x9bc0, 0x75b7, 0xe1fd, 0x3d93, 0x4c26, 0x6c36, 0x7e3f, 0xf5f7, 0x83cc, 0x6834, 0x51a5,
    0xd1e5, 0xf9f1, 0xe271, 0xabd8, 0x6231, 0x2a15, 0x0804, 0x95c7, 0x4623, 0x9dc3, 0x3018, 0x3796, 0x0a05, 0x2f9a,
    0x0e07, 0x2412, 0x1b80, 0xdfe2, 0xcdeb, 0x4e27, 0x7fb2, 0xea75, 0x1209, 0x1d83, 0x582c, 0x341a, 0x361b, 0xdc6e,
    0xb45a, 0x5ba0, 0xa452, 0x763b, 0xb7d6, 0x7db3, 0x5229, 0xdde3, 0x5e2f, 0x1384, 0xa653, 0xb9d1, 0x0,    0xc1ed,
    0x4020, 0xe3fc, 0x79b1, 0xb65b, 0xd46a, 0x8dcb, 0x67be, 0x7239, 0x944a, 0x984c, 0xb058, 0x85cf, 0xbbd0, 0xc5ef,
    0x4faa, 0xedfb, 0x8643, 0x9a4d, 0x6633, 0x1185, 0x8a45, 0xe9f9, 0x402,  0xfe7f, 0xa050, 0x783c, 0x259f, 0x4ba8,
    0xa251, 0x5da3, 0x8040, 0x58f,  0x3f92, 0x219d, 0x7038, 0xf1f5, 0x63bc, 0x77b6, 0xafda, 0x4221, 0x2010, 0xe5ff,
    0xfdf3, 0xbfd2, 0x81cd, 0x180c, 0x2613, 0xc3ec, 0xbe5f, 0x3597, 0x8844, 0x2e17, 0x93c4, 0x55a7, 0xfc7e, 0x7a3d,
    0xc864, 0xba5d, 0x3219, 0xe673, 0xc060, 0x1981, 0x9e4f, 0xa3dc, 0x4422, 0x542a, 0x3b90, 0x0b88, 0x8c46, 0xc7ee,
    0x6bb8, 0x2814, 0xa7de, 0xbc5e, 0x160b, 0xaddb, 0xdbe0, 0x6432, 0x743a, 0x140a, 0x9249, 0x0c06, 0x4824, 0xb85c,
    0x9fc2, 0xbdd3, 0x43ac, 0xc462, 0x3991, 0x3195, 0xd3e4, 0xf279, 0xd5e7, 0x8bc8, 0x6e37, 0xda6d, 0x18d,  0xb1d5,
    0x9c4e, 0x49a9, 0xd86c, 0xac56, 0xf3f4, 0xcfea, 0xca65, 0xf47a, 0x47ae, 0x1008, 0x6fba, 0xf078, 0x4a25, 0x5c2e,
    0x381c, 0x57a6, 0x73b4, 0x97c6, 0xcbe8, 0xa1dd, 0xe874, 0x3e1f, 0x964b, 0x61bd, 0x0d8b, 0x0f8a, 0xe070, 0x7c3e,
    0x71b5, 0xcc66, 0x9048, 0x603,  0xf7f6, 0x1c0e, 0xc261, 0x6a35, 0xae57, 0x69b9, 0x1786, 0x99c1, 0x3a1d, 0x279e,
    0xd9e1, 0xebf8, 0x2b98, 0x2211, 0xd269, 0xa9d9, 0x78e,  0x3394, 0x2d9b, 0x3c1e, 0x1587, 0xc9e9, 0x87ce, 0xaa55,
    0x5028, 0xa5df, 0x38c,  0x59a1, 0x0989, 0x1a0d, 0x65bf, 0xd7e6, 0x8442, 0xd068, 0x8241, 0x2999, 0x5a2d, 0x1e0f,
    0x7bb0, 0xa854, 0x6dbb, 0x2c16};

const ap_uint<8> isbox[256] = {
    0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB, 0x7C, 0xE3, 0x39,
    0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB, 0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2,
    0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E, 0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76,
    0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25, 0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC,
    0x5D, 0x65, 0xB6, 0x92, 0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D,
    0x84, 0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06, 0xD0, 0x2C,
    0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B, 0x3A, 0x91, 0x11, 0x41, 0x4F,
    0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73, 0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85,
    0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E, 0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62,
    0x0E, 0xAA, 0x18, 0xBE, 0x1B, 0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD,
    0x5A, 0xF4, 0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F, 0x60,
    0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF, 0xA0, 0xE0, 0x3B, 0x4D,
    0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61, 0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6,
    0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D};

const ap_uint<8> sbox[2048] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x1,  0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, 0xca, 0x82, 0xc9,
    0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f,
    0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, 0x4,  0xc7, 0x23, 0xc3, 0x18, 0x96, 0x5,  0x9a, 0x7,
    0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3,
    0x29, 0xe3, 0x2f, 0x84, 0x53, 0xd1, 0x0,  0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58,
    0xcf, 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x2,  0x7f, 0x50, 0x3c, 0x9f, 0xa8, 0x51, 0xa3,
    0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, 0xcd, 0x0c, 0x13, 0xec, 0x5f,
    0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
    0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x6,  0x24, 0x5c, 0xc2, 0xd3, 0xac,
    0x62, 0x91, 0x95, 0xe4, 0x79, 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a,
    0xae, 0x08, 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, 0x70,
    0x3e, 0xb5, 0x66, 0x48, 0x3,  0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, 0xe1, 0xf8, 0x98, 0x11,
    0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, 0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42,
    0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16, 0x62, 0x7d, 0x76, 0x7a, 0xf3, 0x6a, 0x6e, 0xc4, 0x31, 0x0,
    0x66, 0x2a, 0xff, 0xd6, 0xaa, 0x77, 0xcb, 0x83, 0xc8, 0x7c, 0xfb, 0x58, 0x46, 0xf1, 0xac, 0xd5, 0xa3, 0xae, 0x9d,
    0xa5, 0x73, 0xc1, 0xb6, 0xfc, 0x92, 0x27, 0x37, 0x3e, 0xf6, 0xcd, 0x35, 0xa4, 0xe4, 0xf0, 0x70, 0xd9, 0x30, 0x14,
    0x5,  0xc6, 0x22, 0xc2, 0x19, 0x97, 0x4,  0x9b, 0x6,  0x13, 0x81, 0xe3, 0xea, 0x26, 0xb3, 0x74, 0x08, 0x82, 0x2d,
    0x1b, 0x1a, 0x6f, 0x5b, 0xa1, 0x53, 0x3a, 0xd7, 0xb2, 0x28, 0xe2, 0x2e, 0x85, 0x52, 0xd0, 0x1,  0xec, 0x21, 0xfd,
    0xb0, 0x5a, 0x6b, 0xca, 0xbf, 0x38, 0x4b, 0x4d, 0x59, 0xce, 0xd1, 0xee, 0xab, 0xfa, 0x42, 0x4c, 0x32, 0x84, 0x44,
    0xf8, 0x3,  0x7e, 0x51, 0x3d, 0x9e, 0xa9, 0x50, 0xa2, 0x41, 0x8e, 0x93, 0x9c, 0x39, 0xf4, 0xbd, 0xb7, 0xdb, 0x20,
    0x11, 0xfe, 0xf2, 0xd3, 0xcc, 0x0d, 0x12, 0xed, 0x5e, 0x96, 0x45, 0x16, 0xc5, 0xa6, 0x7f, 0x3c, 0x65, 0x5c, 0x18,
    0x72, 0x61, 0x80, 0x4e, 0xdd, 0x23, 0x2b, 0x91, 0x89, 0x47, 0xef, 0xb9, 0x15, 0xdf, 0x5f, 0x0a, 0xda, 0xe1, 0x33,
    0x3b, 0x0b, 0x48, 0x7,  0x25, 0x5d, 0xc3, 0xd2, 0xad, 0x63, 0x90, 0x94, 0xe5, 0x78, 0xe6, 0xc9, 0x36, 0x6c, 0x8c,
    0xd4, 0x4f, 0xa8, 0x6d, 0x57, 0xf5, 0xeb, 0x64, 0x7b, 0xaf, 0x09, 0xbb, 0x79, 0x24, 0x2f, 0x1d, 0xa7, 0xb5, 0xc7,
    0xe9, 0xdc, 0x75, 0x1e, 0x4a, 0xbc, 0x8a, 0x8b, 0x71, 0x3f, 0xb4, 0x67, 0x49, 0x2,  0xf7, 0x0f, 0x60, 0x34, 0x56,
    0xb8, 0x87, 0xc0, 0x1c, 0x9f, 0xe0, 0xf9, 0x99, 0x10, 0x68, 0xd8, 0x8f, 0x95, 0x9a, 0x1f, 0x86, 0xe8, 0xcf, 0x54,
    0x29, 0xde, 0x8d, 0xa0, 0x88, 0x0c, 0xbe, 0xe7, 0x43, 0x69, 0x40, 0x98, 0x2c, 0x0e, 0xb1, 0x55, 0xba, 0x17, 0x61,
    0x7e, 0x75, 0x79, 0xf0, 0x69, 0x6d, 0xc7, 0x32, 0x3,  0x65, 0x29, 0xfc, 0xd5, 0xa9, 0x74, 0xc8, 0x80, 0xcb, 0x7f,
    0xf8, 0x5b, 0x45, 0xf2, 0xaf, 0xd6, 0xa0, 0xad, 0x9e, 0xa6, 0x70, 0xc2, 0xb5, 0xff, 0x91, 0x24, 0x34, 0x3d, 0xf5,
    0xce, 0x36, 0xa7, 0xe7, 0xf3, 0x73, 0xda, 0x33, 0x17, 0x6,  0xc5, 0x21, 0xc1, 0x1a, 0x94, 0x7,  0x98, 0x5,  0x10,
    0x82, 0xe0, 0xe9, 0x25, 0xb0, 0x77, 0x0b, 0x81, 0x2e, 0x18, 0x19, 0x6c, 0x58, 0xa2, 0x50, 0x39, 0xd4, 0xb1, 0x2b,
    0xe1, 0x2d, 0x86, 0x51, 0xd3, 0x2,  0xef, 0x22, 0xfe, 0xb3, 0x59, 0x68, 0xc9, 0xbc, 0x3b, 0x48, 0x4e, 0x5a, 0xcd,
    0xd2, 0xed, 0xa8, 0xf9, 0x41, 0x4f, 0x31, 0x87, 0x47, 0xfb, 0x0,  0x7d, 0x52, 0x3e, 0x9d, 0xaa, 0x53, 0xa1, 0x42,
    0x8d, 0x90, 0x9f, 0x3a, 0xf7, 0xbe, 0xb4, 0xd8, 0x23, 0x12, 0xfd, 0xf1, 0xd0, 0xcf, 0x0e, 0x11, 0xee, 0x5d, 0x95,
    0x46, 0x15, 0xc6, 0xa5, 0x7c, 0x3f, 0x66, 0x5f, 0x1b, 0x71, 0x62, 0x83, 0x4d, 0xde, 0x20, 0x28, 0x92, 0x8a, 0x44,
    0xec, 0xba, 0x16, 0xdc, 0x5c, 0x09, 0xd9, 0xe2, 0x30, 0x38, 0x08, 0x4b, 0x4,  0x26, 0x5e, 0xc0, 0xd1, 0xae, 0x60,
    0x93, 0x97, 0xe6, 0x7b, 0xe5, 0xca, 0x35, 0x6f, 0x8f, 0xd7, 0x4c, 0xab, 0x6e, 0x54, 0xf6, 0xe8, 0x67, 0x78, 0xac,
    0x0a, 0xb8, 0x7a, 0x27, 0x2c, 0x1e, 0xa4, 0xb6, 0xc4, 0xea, 0xdf, 0x76, 0x1d, 0x49, 0xbf, 0x89, 0x88, 0x72, 0x3c,
    0xb7, 0x64, 0x4a, 0x1,  0xf4, 0x0c, 0x63, 0x37, 0x55, 0xbb, 0x84, 0xc3, 0x1f, 0x9c, 0xe3, 0xfa, 0x9a, 0x13, 0x6b,
    0xdb, 0x8c, 0x96, 0x99, 0x1c, 0x85, 0xeb, 0xcc, 0x57, 0x2a, 0xdd, 0x8e, 0xa3, 0x8b, 0x0f, 0xbd, 0xe4, 0x40, 0x6a,
    0x43, 0x9b, 0x2f, 0x0d, 0xb2, 0x56, 0xb9, 0x14, 0x67, 0x78, 0x73, 0x7f, 0xf6, 0x6f, 0x6b, 0xc1, 0x34, 0x5,  0x63,
    0x2f, 0xfa, 0xd3, 0xaf, 0x72, 0xce, 0x86, 0xcd, 0x79, 0xfe, 0x5d, 0x43, 0xf4, 0xa9, 0xd0, 0xa6, 0xab, 0x98, 0xa0,
    0x76, 0xc4, 0xb3, 0xf9, 0x97, 0x22, 0x32, 0x3b, 0xf3, 0xc8, 0x30, 0xa1, 0xe1, 0xf5, 0x75, 0xdc, 0x35, 0x11, 0x0,
    0xc3, 0x27, 0xc7, 0x1c, 0x92, 0x1,  0x9e, 0x3,  0x16, 0x84, 0xe6, 0xef, 0x23, 0xb6, 0x71, 0x0d, 0x87, 0x28, 0x1e,
    0x1f, 0x6a, 0x5e, 0xa4, 0x56, 0x3f, 0xd2, 0xb7, 0x2d, 0xe7, 0x2b, 0x80, 0x57, 0xd5, 0x4,  0xe9, 0x24, 0xf8, 0xb5,
    0x5f, 0x6e, 0xcf, 0xba, 0x3d, 0x4e, 0x48, 0x5c, 0xcb, 0xd4, 0xeb, 0xae, 0xff, 0x47, 0x49, 0x37, 0x81, 0x41, 0xfd,
    0x6,  0x7b, 0x54, 0x38, 0x9b, 0xac, 0x55, 0xa7, 0x44, 0x8b, 0x96, 0x99, 0x3c, 0xf1, 0xb8, 0xb2, 0xde, 0x25, 0x14,
    0xfb, 0xf7, 0xd6, 0xc9, 0x08, 0x17, 0xe8, 0x5b, 0x93, 0x40, 0x13, 0xc0, 0xa3, 0x7a, 0x39, 0x60, 0x59, 0x1d, 0x77,
    0x64, 0x85, 0x4b, 0xd8, 0x26, 0x2e, 0x94, 0x8c, 0x42, 0xea, 0xbc, 0x10, 0xda, 0x5a, 0x0f, 0xdf, 0xe4, 0x36, 0x3e,
    0x0e, 0x4d, 0x2,  0x20, 0x58, 0xc6, 0xd7, 0xa8, 0x66, 0x95, 0x91, 0xe0, 0x7d, 0xe3, 0xcc, 0x33, 0x69, 0x89, 0xd1,
    0x4a, 0xad, 0x68, 0x52, 0xf0, 0xee, 0x61, 0x7e, 0xaa, 0x0c, 0xbe, 0x7c, 0x21, 0x2a, 0x18, 0xa2, 0xb0, 0xc2, 0xec,
    0xd9, 0x70, 0x1b, 0x4f, 0xb9, 0x8f, 0x8e, 0x74, 0x3a, 0xb1, 0x62, 0x4c, 0x7,  0xf2, 0x0a, 0x65, 0x31, 0x53, 0xbd,
    0x82, 0xc5, 0x19, 0x9a, 0xe5, 0xfc, 0x9c, 0x15, 0x6d, 0xdd, 0x8a, 0x90, 0x9f, 0x1a, 0x83, 0xed, 0xca, 0x51, 0x2c,
    0xdb, 0x88, 0xa5, 0x8d, 0x09, 0xbb, 0xe2, 0x46, 0x6c, 0x45, 0x9d, 0x29, 0x0b, 0xb4, 0x50, 0xbf, 0x12, 0x6b, 0x74,
    0x7f, 0x73, 0xfa, 0x63, 0x67, 0xcd, 0x38, 0x09, 0x6f, 0x23, 0xf6, 0xdf, 0xa3, 0x7e, 0xc2, 0x8a, 0xc1, 0x75, 0xf2,
    0x51, 0x4f, 0xf8, 0xa5, 0xdc, 0xaa, 0xa7, 0x94, 0xac, 0x7a, 0xc8, 0xbf, 0xf5, 0x9b, 0x2e, 0x3e, 0x37, 0xff, 0xc4,
    0x3c, 0xad, 0xed, 0xf9, 0x79, 0xd0, 0x39, 0x1d, 0x0c, 0xcf, 0x2b, 0xcb, 0x10, 0x9e, 0x0d, 0x92, 0x0f, 0x1a, 0x88,
    0xea, 0xe3, 0x2f, 0xba, 0x7d, 0x1,  0x8b, 0x24, 0x12, 0x13, 0x66, 0x52, 0xa8, 0x5a, 0x33, 0xde, 0xbb, 0x21, 0xeb,
    0x27, 0x8c, 0x5b, 0xd9, 0x08, 0xe5, 0x28, 0xf4, 0xb9, 0x53, 0x62, 0xc3, 0xb6, 0x31, 0x42, 0x44, 0x50, 0xc7, 0xd8,
    0xe7, 0xa2, 0xf3, 0x4b, 0x45, 0x3b, 0x8d, 0x4d, 0xf1, 0x0a, 0x77, 0x58, 0x34, 0x97, 0xa0, 0x59, 0xab, 0x48, 0x87,
    0x9a, 0x95, 0x30, 0xfd, 0xb4, 0xbe, 0xd2, 0x29, 0x18, 0xf7, 0xfb, 0xda, 0xc5, 0x4,  0x1b, 0xe4, 0x57, 0x9f, 0x4c,
    0x1f, 0xcc, 0xaf, 0x76, 0x35, 0x6c, 0x55, 0x11, 0x7b, 0x68, 0x89, 0x47, 0xd4, 0x2a, 0x22, 0x98, 0x80, 0x4e, 0xe6,
    0xb0, 0x1c, 0xd6, 0x56, 0x3,  0xd3, 0xe8, 0x3a, 0x32, 0x2,  0x41, 0x0e, 0x2c, 0x54, 0xca, 0xdb, 0xa4, 0x6a, 0x99,
    0x9d, 0xec, 0x71, 0xef, 0xc0, 0x3f, 0x65, 0x85, 0xdd, 0x46, 0xa1, 0x64, 0x5e, 0xfc, 0xe2, 0x6d, 0x72, 0xa6, 0x0,
    0xb2, 0x70, 0x2d, 0x26, 0x14, 0xae, 0xbc, 0xce, 0xe0, 0xd5, 0x7c, 0x17, 0x43, 0xb5, 0x83, 0x82, 0x78, 0x36, 0xbd,
    0x6e, 0x40, 0x0b, 0xfe, 0x6,  0x69, 0x3d, 0x5f, 0xb1, 0x8e, 0xc9, 0x15, 0x96, 0xe9, 0xf0, 0x90, 0x19, 0x61, 0xd1,
    0x86, 0x9c, 0x93, 0x16, 0x8f, 0xe1, 0xc6, 0x5d, 0x20, 0xd7, 0x84, 0xa9, 0x81, 0x5,  0xb7, 0xee, 0x4a, 0x60, 0x49,
    0x91, 0x25, 0x7,  0xb8, 0x5c, 0xb3, 0x1e, 0x73, 0x6c, 0x67, 0x6b, 0xe2, 0x7b, 0x7f, 0xd5, 0x20, 0x11, 0x77, 0x3b,
    0xee, 0xc7, 0xbb, 0x66, 0xda, 0x92, 0xd9, 0x6d, 0xea, 0x49, 0x57, 0xe0, 0xbd, 0xc4, 0xb2, 0xbf, 0x8c, 0xb4, 0x62,
    0xd0, 0xa7, 0xed, 0x83, 0x36, 0x26, 0x2f, 0xe7, 0xdc, 0x24, 0xb5, 0xf5, 0xe1, 0x61, 0xc8, 0x21, 0x5,  0x14, 0xd7,
    0x33, 0xd3, 0x08, 0x86, 0x15, 0x8a, 0x17, 0x2,  0x90, 0xf2, 0xfb, 0x37, 0xa2, 0x65, 0x19, 0x93, 0x3c, 0x0a, 0x0b,
    0x7e, 0x4a, 0xb0, 0x42, 0x2b, 0xc6, 0xa3, 0x39, 0xf3, 0x3f, 0x94, 0x43, 0xc1, 0x10, 0xfd, 0x30, 0xec, 0xa1, 0x4b,
    0x7a, 0xdb, 0xae, 0x29, 0x5a, 0x5c, 0x48, 0xdf, 0xc0, 0xff, 0xba, 0xeb, 0x53, 0x5d, 0x23, 0x95, 0x55, 0xe9, 0x12,
    0x6f, 0x40, 0x2c, 0x8f, 0xb8, 0x41, 0xb3, 0x50, 0x9f, 0x82, 0x8d, 0x28, 0xe5, 0xac, 0xa6, 0xca, 0x31, 0x0,  0xef,
    0xe3, 0xc2, 0xdd, 0x1c, 0x3,  0xfc, 0x4f, 0x87, 0x54, 0x7,  0xd4, 0xb7, 0x6e, 0x2d, 0x74, 0x4d, 0x09, 0x63, 0x70,
    0x91, 0x5f, 0xcc, 0x32, 0x3a, 0x80, 0x98, 0x56, 0xfe, 0xa8, 0x4,  0xce, 0x4e, 0x1b, 0xcb, 0xf0, 0x22, 0x2a, 0x1a,
    0x59, 0x16, 0x34, 0x4c, 0xd2, 0xc3, 0xbc, 0x72, 0x81, 0x85, 0xf4, 0x69, 0xf7, 0xd8, 0x27, 0x7d, 0x9d, 0xc5, 0x5e,
    0xb9, 0x7c, 0x46, 0xe4, 0xfa, 0x75, 0x6a, 0xbe, 0x18, 0xaa, 0x68, 0x35, 0x3e, 0x0c, 0xb6, 0xa4, 0xd6, 0xf8, 0xcd,
    0x64, 0x0f, 0x5b, 0xad, 0x9b, 0x9a, 0x60, 0x2e, 0xa5, 0x76, 0x58, 0x13, 0xe6, 0x1e, 0x71, 0x25, 0x47, 0xa9, 0x96,
    0xd1, 0x0d, 0x8e, 0xf1, 0xe8, 0x88, 0x1,  0x79, 0xc9, 0x9e, 0x84, 0x8b, 0x0e, 0x97, 0xf9, 0xde, 0x45, 0x38, 0xcf,
    0x9c, 0xb1, 0x99, 0x1d, 0xaf, 0xf6, 0x52, 0x78, 0x51, 0x89, 0x3d, 0x1f, 0xa0, 0x44, 0xab, 0x6,  0x43, 0x5c, 0x57,
    0x5b, 0xd2, 0x4b, 0x4f, 0xe5, 0x10, 0x21, 0x47, 0x0b, 0xde, 0xf7, 0x8b, 0x56, 0xea, 0xa2, 0xe9, 0x5d, 0xda, 0x79,
    0x67, 0xd0, 0x8d, 0xf4, 0x82, 0x8f, 0xbc, 0x84, 0x52, 0xe0, 0x97, 0xdd, 0xb3, 0x6,  0x16, 0x1f, 0xd7, 0xec, 0x14,
    0x85, 0xc5, 0xd1, 0x51, 0xf8, 0x11, 0x35, 0x24, 0xe7, 0x3,  0xe3, 0x38, 0xb6, 0x25, 0xba, 0x27, 0x32, 0xa0, 0xc2,
    0xcb, 0x7,  0x92, 0x55, 0x29, 0xa3, 0x0c, 0x3a, 0x3b, 0x4e, 0x7a, 0x80, 0x72, 0x1b, 0xf6, 0x93, 0x09, 0xc3, 0x0f,
    0xa4, 0x73, 0xf1, 0x20, 0xcd, 0x0,  0xdc, 0x91, 0x7b, 0x4a, 0xeb, 0x9e, 0x19, 0x6a, 0x6c, 0x78, 0xef, 0xf0, 0xcf,
    0x8a, 0xdb, 0x63, 0x6d, 0x13, 0xa5, 0x65, 0xd9, 0x22, 0x5f, 0x70, 0x1c, 0xbf, 0x88, 0x71, 0x83, 0x60, 0xaf, 0xb2,
    0xbd, 0x18, 0xd5, 0x9c, 0x96, 0xfa, 0x1,  0x30, 0xdf, 0xd3, 0xf2, 0xed, 0x2c, 0x33, 0xcc, 0x7f, 0xb7, 0x64, 0x37,
    0xe4, 0x87, 0x5e, 0x1d, 0x44, 0x7d, 0x39, 0x53, 0x40, 0xa1, 0x6f, 0xfc, 0x2,  0x0a, 0xb0, 0xa8, 0x66, 0xce, 0x98,
    0x34, 0xfe, 0x7e, 0x2b, 0xfb, 0xc0, 0x12, 0x1a, 0x2a, 0x69, 0x26, 0x4,  0x7c, 0xe2, 0xf3, 0x8c, 0x42, 0xb1, 0xb5,
    0xc4, 0x59, 0xc7, 0xe8, 0x17, 0x4d, 0xad, 0xf5, 0x6e, 0x89, 0x4c, 0x76, 0xd4, 0xca, 0x45, 0x5a, 0x8e, 0x28, 0x9a,
    0x58, 0x5,  0x0e, 0x3c, 0x86, 0x94, 0xe6, 0xc8, 0xfd, 0x54, 0x3f, 0x6b, 0x9d, 0xab, 0xaa, 0x50, 0x1e, 0x95, 0x46,
    0x68, 0x23, 0xd6, 0x2e, 0x41, 0x15, 0x77, 0x99, 0xa6, 0xe1, 0x3d, 0xbe, 0xc1, 0xd8, 0xb8, 0x31, 0x49, 0xf9, 0xae,
    0xb4, 0xbb, 0x3e, 0xa7, 0xc9, 0xee, 0x75, 0x08, 0xff, 0xac, 0x81, 0xa9, 0x2d, 0x9f, 0xc6, 0x62, 0x48, 0x61, 0xb9,
    0x0d, 0x2f, 0x90, 0x74, 0x9b, 0x36, 0x23, 0x3c, 0x37, 0x3b, 0xb2, 0x2b, 0x2f, 0x85, 0x70, 0x41, 0x27, 0x6b, 0xbe,
    0x97, 0xeb, 0x36, 0x8a, 0xc2, 0x89, 0x3d, 0xba, 0x19, 0x7,  0xb0, 0xed, 0x94, 0xe2, 0xef, 0xdc, 0xe4, 0x32, 0x80,
    0xf7, 0xbd, 0xd3, 0x66, 0x76, 0x7f, 0xb7, 0x8c, 0x74, 0xe5, 0xa5, 0xb1, 0x31, 0x98, 0x71, 0x55, 0x44, 0x87, 0x63,
    0x83, 0x58, 0xd6, 0x45, 0xda, 0x47, 0x52, 0xc0, 0xa2, 0xab, 0x67, 0xf2, 0x35, 0x49, 0xc3, 0x6c, 0x5a, 0x5b, 0x2e,
    0x1a, 0xe0, 0x12, 0x7b, 0x96, 0xf3, 0x69, 0xa3, 0x6f, 0xc4, 0x13, 0x91, 0x40, 0xad, 0x60, 0xbc, 0xf1, 0x1b, 0x2a,
    0x8b, 0xfe, 0x79, 0x0a, 0x0c, 0x18, 0x8f, 0x90, 0xaf, 0xea, 0xbb, 0x3,  0x0d, 0x73, 0xc5, 0x5,  0xb9, 0x42, 0x3f,
    0x10, 0x7c, 0xdf, 0xe8, 0x11, 0xe3, 0x0,  0xcf, 0xd2, 0xdd, 0x78, 0xb5, 0xfc, 0xf6, 0x9a, 0x61, 0x50, 0xbf, 0xb3,
    0x92, 0x8d, 0x4c, 0x53, 0xac, 0x1f, 0xd7, 0x4,  0x57, 0x84, 0xe7, 0x3e, 0x7d, 0x24, 0x1d, 0x59, 0x33, 0x20, 0xc1,
    0x0f, 0x9c, 0x62, 0x6a, 0xd0, 0xc8, 0x6,  0xae, 0xf8, 0x54, 0x9e, 0x1e, 0x4b, 0x9b, 0xa0, 0x72, 0x7a, 0x4a, 0x09,
    0x46, 0x64, 0x1c, 0x82, 0x93, 0xec, 0x22, 0xd1, 0xd5, 0xa4, 0x39, 0xa7, 0x88, 0x77, 0x2d, 0xcd, 0x95, 0x0e, 0xe9,
    0x2c, 0x16, 0xb4, 0xaa, 0x25, 0x3a, 0xee, 0x48, 0xfa, 0x38, 0x65, 0x6e, 0x5c, 0xe6, 0xf4, 0x86, 0xa8, 0x9d, 0x34,
    0x5f, 0x0b, 0xfd, 0xcb, 0xca, 0x30, 0x7e, 0xf5, 0x26, 0x08, 0x43, 0xb6, 0x4e, 0x21, 0x75, 0x17, 0xf9, 0xc6, 0x81,
    0x5d, 0xde, 0xa1, 0xb8, 0xd8, 0x51, 0x29, 0x99, 0xce, 0xd4, 0xdb, 0x5e, 0xc7, 0xa9, 0x8e, 0x15, 0x68, 0x9f, 0xcc,
    0xe1, 0xc9, 0x4d, 0xff, 0xa6, 0x2,  0x28, 0x1,  0xd9, 0x6d, 0x4f, 0xf0, 0x14, 0xfb, 0x56};

const ap_uint<32> gtbox[256] = {
    0x50a7f451, 0x5365417e, 0xc3a4171a, 0x965e273a, 0xcb6bab3b, 0xf1459d1f, 0xab58faac, 0x9303e34b, 0x55fa3020,
    0xf66d76ad, 0x9176cc88, 0x254c02f5, 0xfcd7e54f, 0xd7cb2ac5, 0x80443526, 0x8fa362b5, 0x495ab1de, 0x671bba25,
    0x980eea45, 0xe1c0fe5d, 0x02752fc3, 0x12f04c81, 0xa397468d, 0xc6f9d36b, 0xe75f8f03, 0x959c9215, 0xeb7a6dbf,
    0xda595295, 0x2d83bed4, 0xd3217458, 0x2969e049, 0x44c8c98e, 0x6a89c275, 0x78798ef4, 0x6b3e5899, 0xdd71b927,
    0xb64fe1be, 0x17ad88f0, 0x66ac20c9, 0xb43ace7d, 0x184adf63, 0x82311ae5, 0x60335197, 0x457f5362, 0xe07764b1,
    0x84ae6bbb, 0x1ca081fe, 0x942b08f9, 0x58684870, 0x19fd458f, 0x876cde94, 0xb7f87b52, 0x23d373ab, 0xe2024b72,
    0x578f1fe3, 0x2aab5566, 0x0728ebb2, 0x03c2b52f, 0x9a7bc586, 0xa50837d3, 0xf2872830, 0xb2a5bf23, 0xba6a0302,
    0x5c8216ed, 0x2b1ccf8a, 0x92b479a7, 0xf0f207f3, 0xa1e2694e, 0xcdf4da65, 0xd5be0506, 0x1f6234d1, 0x8afea6c4,
    0x9d532e34, 0xa055f3a2, 0x32e18a05, 0x75ebf6a4, 0x39ec830b, 0xaaef6040, 0x069f715e, 0x51106ebd, 0xf98a213e,
    0x3d06dd96, 0xae053edd, 0x46bde64d, 0xb58d5491, 0x055dc471, 0x6fd40604, 0xff155060, 0x24fb9819, 0x97e9bdd6,
    0xcc434089, 0x779ed967, 0xbd42e8b0, 0x888b8907, 0x385b19e7, 0xdbeec879, 0x470a7ca1, 0xe90f427c, 0xc91e84f8,
    0x00000000, 0x83868009, 0x48ed2b32, 0xac70111e, 0x4e725a6c, 0xfbff0efd, 0x5638850f, 0x1ed5ae3d, 0x27392d36,
    0x64d90f0a, 0x21a65c68, 0xd1545b9b, 0x3a2e3624, 0xb1670a0c, 0x0fe75793, 0xd296eeb4, 0x9e919b1b, 0x4fc5c080,
    0xa220dc61, 0x694b775a, 0x161a121c, 0x0aba93e2, 0xe52aa0c0, 0x43e0223c, 0x1d171b12, 0x0b0d090e, 0xadc78bf2,
    0xb9a8b62d, 0xc8a91e14, 0x8519f157, 0x4c0775af, 0xbbdd99ee, 0xfd607fa3, 0x9f2601f7, 0xbcf5725c, 0xc53b6644,
    0x347efb5b, 0x7629438b, 0xdcc623cb, 0x68fcedb6, 0x63f1e4b8, 0xcadc31d7, 0x10856342, 0x40229713, 0x2011c684,
    0x7d244a85, 0xf83dbbd2, 0x1132f9ae, 0x6da129c7, 0x4b2f9e1d, 0xf330b2dc, 0xec52860d, 0xd0e3c177, 0x6c16b32b,
    0x99b970a9, 0xfa489411, 0x2264e947, 0xc48cfca8, 0x1a3ff0a0, 0xd82c7d56, 0xef903322, 0xc74e4987, 0xc1d138d9,
    0xfea2ca8c, 0x360bd498, 0xcf81f5a6, 0x28de7aa5, 0x268eb7da, 0xa4bfad3f, 0xe49d3a2c, 0x0d927850, 0x9bcc5f6a,
    0x62467e54, 0xc2138df6, 0xe8b8d890, 0x5ef7392e, 0xf5afc382, 0xbe805d9f, 0x7c93d069, 0xa92dd56f, 0xb31225cf,
    0x3b99acc8, 0xa77d1810, 0x6e639ce8, 0x7bbb3bdb, 0x097826cd, 0xf418596e, 0x01b79aec, 0xa89a4f83, 0x656e95e6,
    0x7ee6ffaa, 0x08cfbc21, 0xe6e815ef, 0xd99be7ba, 0xce366f4a, 0xd4099fea, 0xd67cb029, 0xafb2a431, 0x31233f2a,
    0x3094a5c6, 0xc066a235, 0x37bc4e74, 0xa6ca82fc, 0xb0d090e0, 0x15d8a733, 0x4a9804f1, 0xf7daec41, 0x0e50cd7f,
    0x2ff69117, 0x8dd64d76, 0x4db0ef43, 0x544daacc, 0xdf0496e4, 0xe3b5d19e, 0x1b886a4c, 0xb81f2cc1, 0x7f516546,
    0x04ea5e9d, 0x5d358c01, 0x737487fa, 0x2e410bfb, 0x5a1d67b3, 0x52d2db92, 0x335610e9, 0x1347d66d, 0x8c61d79a,
    0x7a0ca137, 0x8e14f859, 0x893c13eb, 0xee27a9ce, 0x35c961b7, 0xede51ce1, 0x3cb1477a, 0x59dfd29c, 0x3f73f255,
    0x79ce1418, 0xbf37c773, 0xeacdf753, 0x5baafd5f, 0x146f3ddf, 0x86db4478, 0x81f3afca, 0x3ec468b9, 0x2c342438,
    0x5f40a3c2, 0x72c31d16, 0x0c25e2bc, 0x8b493c28, 0x41950dff, 0x7101a839, 0xdeb30c08, 0x9ce4b4d8, 0x90c15664,
    0x6184cb7b, 0x70b632d5, 0x745c6c48, 0x4257b8d0};

const ap_uint<8> sboxEnc[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, 0xca, 0x82, 0xc9,
    0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f,
    0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, 0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07,
    0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3,
    0x29, 0xe3, 0x2f, 0x84, 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58,
    0xcf, 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, 0x51, 0xa3,
    0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, 0xcd, 0x0c, 0x13, 0xec, 0x5f,
    0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
    0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac,
    0x62, 0x91, 0x95, 0xe4, 0x79, 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a,
    0xae, 0x08, 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, 0x70,
    0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, 0xe1, 0xf8, 0x98, 0x11,
    0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, 0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42,
    0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};

template <int W>
class aesEnc {
   public:
    void updateKey(ap_uint<W> cipherkey) {}
    void process(ap_uint<128> plaintext, ap_uint<W> cipherkey, ap_uint<128>& ciphertext) {}
};

template <>
class aesEnc<256> {
   public:
    ap_uint<128> key_list[15];

    aesEnc() {
#pragma HLS resource variable = sbox_mix_col_1 core = ROM_nP_LUTRAM
#pragma HLS resource variable = sbox_Rcon core = ROM_2P_BRAM
#pragma HLS ARRAY_PARTITION variable = key_list complete dim = 1
    }

    void updateKey(ap_uint<256> cipherkey) {
#pragma HLS inline off
        ap_uint<128> roundkey, roundkey_1, roundkey_2;
        ap_uint<32> round_tmp;
        ap_uint<8> round_shift_tmp;
        ap_uint<11> addr;

        roundkey_2(127, 0) = cipherkey(127, 0);
        roundkey_1(127, 0) = cipherkey(255, 128);
        key_list[0] = roundkey_2;
        for (ap_uint<4> round_counter = 1; round_counter < 15; round_counter++) {
#pragma HLS PIPELINE
            if (round_counter == 1) {
                roundkey = roundkey_1;
            } else {
                round_tmp(31, 0) = roundkey_1(127, 96);
                // if round_counter %2 == 0, right circular shift
                if (!round_counter[0]) {
                    round_shift_tmp(7, 0) = round_tmp(7, 0);
                    round_tmp >>= 8;
                    round_tmp(31, 24) = round_shift_tmp(7, 0);
                    addr(10, 8) = round_counter(3, 1);
                } else {
                    addr(10, 8) = 0;
                }

                addr(7, 0) = round_tmp(7, 0);

                round_tmp(7, 0) = sbox_Rcon[addr];
                round_tmp(15, 8) = sbox_Rcon[round_tmp(15, 8)];
                round_tmp(23, 16) = sbox_Rcon[round_tmp(23, 16)];
                round_tmp(31, 24) = sbox_Rcon[round_tmp(31, 24)];

                roundkey(31, 0) = roundkey_2(31, 0) ^ round_tmp;
                roundkey(63, 32) = roundkey_2(63, 32) ^ roundkey(31, 0);
                roundkey(95, 64) = roundkey_2(95, 64) ^ roundkey(63, 32);
                roundkey(127, 96) = roundkey_2(127, 96) ^ roundkey(95, 64);

                roundkey_2 = roundkey_1;
                roundkey_1 = roundkey;
            }
            key_list[round_counter] = roundkey;
        }
    }

    void process(ap_uint<128> plaintext, ap_uint<256> cipherkey, ap_uint<128>& ciphertext) {
#pragma HLS inline off
        ap_uint<128> state, state_1, state_2, state_3;
        ap_uint<8> tmp_1, tmp_2_1, tmp_2_2, tmp_3;
        ap_uint<4> round_counter;
        ap_uint<16> mix_col_1_tmp;

        // state init and add roundkey[0]
        state = plaintext ^ key_list[0];

        // Start 14 rounds of process
        for (round_counter = 1; round_counter <= 14; round_counter++) {
            // ShiftRow
            tmp_1 = state(15, 8);
            state(15, 8) = state(47, 40);
            state(47, 40) = state(79, 72);
            state(79, 72) = state(111, 104);
            state(111, 104) = tmp_1;

            tmp_2_1 = state(23, 16);
            state(23, 16) = state(87, 80);
            state(87, 80) = tmp_2_1;

            tmp_2_2 = state(55, 48);
            state(55, 48) = state(119, 112);
            state(119, 112) = tmp_2_2;

            tmp_3 = state(127, 120);
            state(127, 120) = state(95, 88);
            state(95, 88) = state(63, 56);
            state(63, 56) = state(31, 24);
            state(31, 24) = tmp_3;

            // SubByte and MixColumn
            for (int i = 0; i < 16; i++) {
#pragma HLS unroll

                mix_col_1_tmp = sbox_mix_col_1[state(i * 8 + 7, i * 8)];
                state_1(i * 8 + 7, i * 8) = mix_col_1_tmp(7, 0);
                state_2(i * 8 + 7, i * 8) = mix_col_1_tmp(15, 8);
                state_3(i * 8 + 7, i * 8) = state_1(i * 8 + 7, i * 8) ^ state_2(i * 8 + 7, i * 8);
            }
            if (round_counter < 14) {
                for (int i = 0; i < 4; i++) {
#pragma HLS unroll

                    state(i * 32 + 7, i * 32) = state_2(i * 32 + 7, i * 32) ^ state_3(i * 32 + 15, i * 32 + 8) ^
                                                state_1(i * 32 + 23, i * 32 + 16) ^ state_1(i * 32 + 31, i * 32 + 24);
                    state(i * 32 + 15, i * 32 + 8) = state_1(i * 32 + 7, i * 32) ^ state_2(i * 32 + 15, i * 32 + 8) ^
                                                     state_3(i * 32 + 23, i * 32 + 16) ^
                                                     state_1(i * 32 + 31, i * 32 + 24);
                    state(i * 32 + 23, i * 32 + 16) = state_1(i * 32 + 7, i * 32) ^ state_1(i * 32 + 15, i * 32 + 8) ^
                                                      state_2(i * 32 + 23, i * 32 + 16) ^
                                                      state_3(i * 32 + 31, i * 32 + 24);
                    state(i * 32 + 31, i * 32 + 24) = state_3(i * 32 + 7, i * 32) ^ state_1(i * 32 + 15, i * 32 + 8) ^
                                                      state_1(i * 32 + 23, i * 32 + 16) ^
                                                      state_2(i * 32 + 31, i * 32 + 24);
                }
            } else {
                state = state_1;
            }

            state ^= key_list[round_counter];
        }
        ciphertext = state;
    }
};

/**
 * @brief aes256Encrypt is the basic function for ciphering one block
with one cipher key
 *
 * @param plaintext input one word plain text to be encrypted, 128 bits.
 * @param cipherkey input cipher key, 256 bits.
 * @param ciphertext output one word encrypted text, 128 bit.
 *
 */
static void aes256Encrypt(ap_uint<128> plaintext, ap_uint<256> cipherkey, ap_uint<128>& ciphertext) {
#pragma HLS PIPELINE II = 1

#pragma HLS resource variable = sbox_mix_col_1 core = ROM_nP_LUTRAM
#pragma HLS resource variable = isbox core = ROM_nP_LUTRAM
#pragma HLS resource variable = sbox core = ROM_nP_LUTRAM
#pragma HLS resource variable = gtbox core = ROM_nP_LUTRAM
#pragma HLS resource variable = sboxEnc core = ROM_nP_LUTRAM

    ap_uint<128> state, state_1, state_2, state_3;
    ap_uint<128> roundkey, roundkey_1, roundkey_2;
    ap_uint<32> round_tmp;
    ap_uint<8> round_shift_tmp;
    ap_uint<8> tmp_1, tmp_2_1, tmp_2_2, tmp_3;
    ap_uint<4> round_counter;
    ap_uint<11> addr;
    ap_uint<16> mix_col_1_tmp;

    // Cut cypherkey into roundkey[0] and roundkey[1]
    roundkey_2(127, 0) = cipherkey(127, 0);
    roundkey_1(127, 0) = cipherkey(255, 128);

    // state init and add roundkey[0]
    state = plaintext ^ cipherkey(127, 0);

    // Start 14 rounds of process
    for (round_counter = 1; round_counter <= 14; round_counter++) {
        // Generate roundkey
        if (round_counter == 1) {
            roundkey = roundkey_1;
        } else {
            round_tmp(31, 0) = roundkey_1(127, 96);
            // if round_counter %2 == 0, right circular shift
            if (!round_counter[0]) {
                round_shift_tmp(7, 0) = round_tmp(7, 0);
                round_tmp >>= 8;
                round_tmp(31, 24) = round_shift_tmp(7, 0);
                addr(10, 8) = round_counter(3, 1);
            } else {
                addr(10, 8) = 0;
            }

            addr(7, 0) = round_tmp(7, 0);

            round_tmp(7, 0) = sbox_Rcon[addr];
            round_tmp(15, 8) = sbox_Rcon[round_tmp(15, 8)];
            round_tmp(23, 16) = sbox_Rcon[round_tmp(23, 16)];
            round_tmp(31, 24) = sbox_Rcon[round_tmp(31, 24)];

            roundkey(31, 0) = roundkey_2(31, 0) ^ round_tmp;
            roundkey(63, 32) = roundkey_2(63, 32) ^ roundkey(31, 0);
            roundkey(95, 64) = roundkey_2(95, 64) ^ roundkey(63, 32);
            roundkey(127, 96) = roundkey_2(127, 96) ^ roundkey(95, 64);

            roundkey_2 = roundkey_1;
            roundkey_1 = roundkey;
        }

        // ShiftRow
        tmp_1 = state(15, 8);
        state(15, 8) = state(47, 40);
        state(47, 40) = state(79, 72);
        state(79, 72) = state(111, 104);
        state(111, 104) = tmp_1;

        tmp_2_1 = state(23, 16);
        state(23, 16) = state(87, 80);
        state(87, 80) = tmp_2_1;

        tmp_2_2 = state(55, 48);
        state(55, 48) = state(119, 112);
        state(119, 112) = tmp_2_2;

        tmp_3 = state(127, 120);
        state(127, 120) = state(95, 88);
        state(95, 88) = state(63, 56);
        state(63, 56) = state(31, 24);
        state(31, 24) = tmp_3;

        // SubByte and MixColumn
        for (int i = 0; i < 16; i++) {
#pragma HLS unroll

            mix_col_1_tmp = sbox_mix_col_1[state(i * 8 + 7, i * 8)];
            state_1(i * 8 + 7, i * 8) = mix_col_1_tmp(7, 0);
            state_2(i * 8 + 7, i * 8) = mix_col_1_tmp(15, 8);
            state_3(i * 8 + 7, i * 8) = state_1(i * 8 + 7, i * 8) ^ state_2(i * 8 + 7, i * 8);
        }
        if (round_counter < 14) {
            for (int i = 0; i < 4; i++) {
#pragma HLS unroll

                state(i * 32 + 7, i * 32) = state_2(i * 32 + 7, i * 32) ^ state_3(i * 32 + 15, i * 32 + 8) ^
                                            state_1(i * 32 + 23, i * 32 + 16) ^ state_1(i * 32 + 31, i * 32 + 24);
                state(i * 32 + 15, i * 32 + 8) = state_1(i * 32 + 7, i * 32) ^ state_2(i * 32 + 15, i * 32 + 8) ^
                                                 state_3(i * 32 + 23, i * 32 + 16) ^ state_1(i * 32 + 31, i * 32 + 24);
                state(i * 32 + 23, i * 32 + 16) = state_1(i * 32 + 7, i * 32) ^ state_1(i * 32 + 15, i * 32 + 8) ^
                                                  state_2(i * 32 + 23, i * 32 + 16) ^ state_3(i * 32 + 31, i * 32 + 24);
                state(i * 32 + 31, i * 32 + 24) = state_3(i * 32 + 7, i * 32) ^ state_1(i * 32 + 15, i * 32 + 8) ^
                                                  state_1(i * 32 + 23, i * 32 + 16) ^ state_2(i * 32 + 31, i * 32 + 24);
            }
        } else {
            state = state_1;
        }

        state ^= roundkey;
    }
    ciphertext = state;
    return;
}

template <int W>
class aesDec {
   public:
    void updateKey(ap_uint<W> cipherkey) {}
    void process(ap_uint<128> ciphertext, ap_uint<W> cipherkey, ap_uint<128>& plaintext) {}
};

template <>
class aesDec<256> {
   public:
    ap_uint<128> key_list[15];

    aesDec() {
#pragma HLS resource variable = isbox core = ROM_nP_LUTRAM
#pragma HLS resource variable = gtbox core = ROM_nP_LUTRAM
#pragma HLS resource variable = sbox core = ROM_2P_BRAM
#pragma HLS ARRAY_PARTITION variable = key_list complete dim = 1
    }

    void updateKey(ap_uint<256> cipherkey) {
#pragma HLS inline off
        ap_uint<128> roundkey_1, roundkey_2, roundkey_tmp;
        ap_uint<32> round_tmp;
        ap_uint<11> addr;

        roundkey_2 = cipherkey(127, 0);
        roundkey_1 = cipherkey(255, 128);
        key_list[0] = cipherkey(127, 0);

    key_gen_loop:
        for (ap_uint<4> i = 1; i < 15; i++) {
#pragma HLS unroll
            if (i == 1) {
                roundkey_tmp = roundkey_1;
            } else {
                round_tmp(31, 0) = roundkey_1(127, 96);
                if (!i[0]) { // g function
                    ap_uint<8> round_shift_tmp = round_tmp(7, 0);
                    round_tmp >>= 8;
                    round_tmp(31, 24) = round_shift_tmp(7, 0);
                    addr(10, 8) = i(3, 1);
                } else { // h function
                    addr(10, 8) = 0;
                }
                addr(7, 0) = round_tmp(7, 0);

                round_tmp(7, 0) = sbox[addr];
                round_tmp(15, 8) = sbox[round_tmp(15, 8)];
                round_tmp(23, 16) = sbox[round_tmp(23, 16)];
                round_tmp(31, 24) = sbox[round_tmp(31, 24)];

                roundkey_tmp(31, 0) = roundkey_2(31, 0) ^ round_tmp;
                roundkey_tmp(63, 32) = roundkey_2(63, 32) ^ roundkey_tmp(31, 0);
                roundkey_tmp(95, 64) = roundkey_2(95, 64) ^ roundkey_tmp(63, 32);
                roundkey_tmp(127, 96) = roundkey_2(127, 96) ^ roundkey_tmp(95, 64);

                roundkey_2 = roundkey_1;
                roundkey_1 = roundkey_tmp;
            }

#if !defined(__SYNTHESIS__) && XF_SECURITY_DECRYPT_DEBUG == 1
            std::cout << std::dec << "i=" << i << std::hex << ",round_key:" << roundkey_tmp << std::endl;
#endif

            if (i == 14) {
                key_list[i] = roundkey_tmp;
            } else { // inverse mix column
                ap_uint<128> k_9, k_b, k_d, k_e;
                for (int j = 0; j < 16; j++) {
#pragma HLS unroll
                    ap_uint<8> rk_sbox2 = sbox[roundkey_tmp(j * 8 + 7, j * 8)];
                    ap_uint<32> rk_tbox = gtbox[rk_sbox2];
                    k_9(j * 8 + 7, j * 8) = rk_tbox(15, 8);
                    k_b(j * 8 + 7, j * 8) = rk_tbox(31, 24);
                    k_d(j * 8 + 7, j * 8) = rk_tbox(23, 16);
                    k_e(j * 8 + 7, j * 8) = rk_tbox(7, 0);
                }

                ap_uint<128> key_item_tmp;
                for (int j = 0; j < 4; j++) {
#pragma HLS unroll
                    key_item_tmp(j * 32 + 7, j * 32) = k_e(j * 32 + 7, j * 32) ^ k_b(j * 32 + 15, j * 32 + 8) ^
                                                       k_d(j * 32 + 23, j * 32 + 16) ^ k_9(j * 32 + 31, j * 32 + 24);
                    key_item_tmp(j * 32 + 15, j * 32 + 8) = k_9(j * 32 + 7, j * 32) ^ k_e(j * 32 + 15, j * 32 + 8) ^
                                                            k_b(j * 32 + 23, j * 32 + 16) ^
                                                            k_d(j * 32 + 31, j * 32 + 24);
                    key_item_tmp(j * 32 + 23, j * 32 + 16) = k_d(j * 32 + 7, j * 32) ^ k_9(j * 32 + 15, j * 32 + 8) ^
                                                             k_e(j * 32 + 23, j * 32 + 16) ^
                                                             k_b(j * 32 + 31, j * 32 + 24);
                    key_item_tmp(j * 32 + 31, j * 32 + 24) = k_b(j * 32 + 7, j * 32) ^ k_d(j * 32 + 15, j * 32 + 8) ^
                                                             k_9(j * 32 + 23, j * 32 + 16) ^
                                                             k_e(j * 32 + 31, j * 32 + 24);
                }

                key_list[i] = key_item_tmp;
            }
        }
    }

    void process(ap_uint<128> ciphertext, ap_uint<256> cipherkey, ap_uint<128>& plaintext) {
#pragma HLS inline off
        ap_uint<128> state = ciphertext ^ key_list[14];
    decrypt_loop:
        for (ap_uint<4> rnd = 1; rnd < 15; rnd++) {
#pragma HLS unroll
#if !defined(__SYNTHESIS__) && XF_SECURITY_DECRYPT_DEBUG == 1
            std::cout << std::endl << std::hex << "is_start:" << state << std::endl;
#endif
            // inverse Shift Row
            ap_uint<8> tmp_1 = state(111, 104);
            state(111, 104) = state(79, 72);
            state(79, 72) = state(47, 40);
            state(47, 40) = state(15, 8);
            state(15, 8) = tmp_1;

            ap_uint<8> tmp_2_1, tmp_2_2;
            tmp_2_1 = state(119, 112);
            state(119, 112) = state(55, 48);
            state(55, 48) = tmp_2_1;
            tmp_2_2 = state(87, 80);
            state(87, 80) = state(23, 16);
            state(23, 16) = tmp_2_2;

            ap_uint<8> tmp_3 = state(31, 24);
            state(31, 24) = state(63, 56);
            state(63, 56) = state(95, 88);
            state(95, 88) = state(127, 120);
            state(127, 120) = tmp_3;

#if !defined(__SYNTHESIS__) && XF_SECURITY_DECRYPT_DEBUG == 1
            std::cout << std::hex << "is_row:" << state << std::endl;
#endif
            // inverse subbyte and mix column
            ap_uint<128> k_9, k_b, k_d, k_e;
            for (int j = 0; j < 16; j++) {
#pragma HLS unroll
                ap_uint<32> rk_tbox = gtbox[state(j * 8 + 7, j * 8)];
                k_9(j * 8 + 7, j * 8) = rk_tbox(15, 8);
                k_b(j * 8 + 7, j * 8) = rk_tbox(31, 24);
                k_d(j * 8 + 7, j * 8) = rk_tbox(23, 16);
                k_e(j * 8 + 7, j * 8) = rk_tbox(7, 0);
            }

            if (rnd < 14) {
                for (int j = 0; j < 4; j++) {
#pragma HLS unroll
                    state(j * 32 + 7, j * 32) = k_e(j * 32 + 7, j * 32) ^ k_b(j * 32 + 15, j * 32 + 8) ^
                                                k_d(j * 32 + 23, j * 32 + 16) ^ k_9(j * 32 + 31, j * 32 + 24);
                    state(j * 32 + 15, j * 32 + 8) = k_9(j * 32 + 7, j * 32) ^ k_e(j * 32 + 15, j * 32 + 8) ^
                                                     k_b(j * 32 + 23, j * 32 + 16) ^ k_d(j * 32 + 31, j * 32 + 24);
                    state(j * 32 + 23, j * 32 + 16) = k_d(j * 32 + 7, j * 32) ^ k_9(j * 32 + 15, j * 32 + 8) ^
                                                      k_e(j * 32 + 23, j * 32 + 16) ^ k_b(j * 32 + 31, j * 32 + 24);
                    state(j * 32 + 31, j * 32 + 24) = k_b(j * 32 + 7, j * 32) ^ k_d(j * 32 + 15, j * 32 + 8) ^
                                                      k_9(j * 32 + 23, j * 32 + 16) ^ k_e(j * 32 + 31, j * 32 + 24);
                }
            } else {
                ap_uint<128> state_copy = state;
                for (int j = 0; j < 16; j++) {
#pragma HLS unroll
                    state(j * 8 + 7, j * 8) = isbox[state_copy(j * 8 + 7, j * 8)];
                }
            }

#if !defined(__SYNTHESIS__) && XF_SECURITY_DECRYPT_DEBUG == 1
            std::cout << std::hex << "is_mbox:" << state << std::endl;
#endif
            state ^= key_list[14 - rnd];
#if !defined(__SYNTHESIS__) && XF_SECURITY_DECRYPT_DEBUG == 1
            std::cout << std::hex << "is_last:" << state << std::endl;
#endif
        }
        plaintext = state;
        return;
    }
};

/**
 * @brief aes256Decrypt is the basic function for decrypting one block
with one cipher key
 *
 * @param ciphertext input one word cipher text to be decrypted, 128 bits.
 * @param plainkey input plain key, 256 bits.
 * @param plaintext output one word encrypted text, 128 bit.
 *
 */
static void aes256Decrypt(ap_uint<128> ciphertext, ap_uint<256> cipherkey, ap_uint<128>& plaintext) {
#pragma HLS resource variable = sbox_mix_col_1 core = ROM_nP_LUTRAM
#pragma HLS resource variable = isbox core = ROM_nP_LUTRAM
#pragma HLS resource variable = sbox core = ROM_nP_LUTRAM
#pragma HLS resource variable = gtbox core = ROM_nP_LUTRAM
#pragma HLS resource variable = sboxEnc core = ROM_nP_LUTRAM
    //#pragma HLS RESOURCE variable = sbox core = ROM_nP_LUTRAM
    //#pragma HLS RESOURCE variable = isbox core = ROM_nP_LUTRAM
    //#pragma HLS RESOURCE variable = gtbox core = ROM_nP_LUTRAM
    static ap_uint<128> key_list[15];
#pragma HLS ARRAY_PARTITION variable = key_list complete dim = 1
    static bool is_first = true;
    static bool update_key_en;
    static ap_uint<256> ckey;
    ap_uint<128> roundkey_1, roundkey_2, roundkey_tmp;
    ap_uint<32> round_tmp;
    ap_uint<11> addr;

    // if key requires be updated
    if (ckey != cipherkey)
        update_key_en = true;
    else
        update_key_en = false;

    // Generate all other keys
    if (update_key_en || is_first) {
        is_first = false;
        ckey = cipherkey;
        roundkey_2 = cipherkey(127, 0);
        roundkey_1 = cipherkey(255, 128);
        key_list[0] = cipherkey(127, 0);

    key_gen_loop:
        for (ap_uint<4> i = 1; i < 15; i++) {
#pragma HLS unroll
            if (i == 1) {
                roundkey_tmp = roundkey_1;
            } else {
                round_tmp(31, 0) = roundkey_1(127, 96);
                if (!i[0]) { // g function
                    ap_uint<8> round_shift_tmp = round_tmp(7, 0);
                    round_tmp >>= 8;
                    round_tmp(31, 24) = round_shift_tmp(7, 0);
                    addr(10, 8) = i(3, 1);
                } else { // h function
                    addr(10, 8) = 0;
                }
                addr(7, 0) = round_tmp(7, 0);

                round_tmp(7, 0) = sbox[addr];
                round_tmp(15, 8) = sbox[round_tmp(15, 8)];
                round_tmp(23, 16) = sbox[round_tmp(23, 16)];
                round_tmp(31, 24) = sbox[round_tmp(31, 24)];

                roundkey_tmp(31, 0) = roundkey_2(31, 0) ^ round_tmp;
                roundkey_tmp(63, 32) = roundkey_2(63, 32) ^ roundkey_tmp(31, 0);
                roundkey_tmp(95, 64) = roundkey_2(95, 64) ^ roundkey_tmp(63, 32);
                roundkey_tmp(127, 96) = roundkey_2(127, 96) ^ roundkey_tmp(95, 64);

                roundkey_2 = roundkey_1;
                roundkey_1 = roundkey_tmp;
            }

#if !defined(__SYNTHESIS__) && XF_SECURITY_DECRYPT_DEBUG == 1
            std::cout << std::dec << "i=" << i << std::hex << ",round_key:" << roundkey_tmp << std::endl;
#endif

            if (i == 14) {
                key_list[i] = roundkey_tmp;
            } else { // inverse mix column
                ap_uint<128> k_9, k_b, k_d, k_e;
                for (int j = 0; j < 16; j++) {
#pragma HLS unroll
                    ap_uint<8> rk_sbox2 = sbox[roundkey_tmp(j * 8 + 7, j * 8)];
                    ap_uint<32> rk_tbox = gtbox[rk_sbox2];
                    k_9(j * 8 + 7, j * 8) = rk_tbox(15, 8);
                    k_b(j * 8 + 7, j * 8) = rk_tbox(31, 24);
                    k_d(j * 8 + 7, j * 8) = rk_tbox(23, 16);
                    k_e(j * 8 + 7, j * 8) = rk_tbox(7, 0);
                }

                ap_uint<128> key_item_tmp;
                for (int j = 0; j < 4; j++) {
#pragma HLS unroll
                    key_item_tmp(j * 32 + 7, j * 32) = k_e(j * 32 + 7, j * 32) ^ k_b(j * 32 + 15, j * 32 + 8) ^
                                                       k_d(j * 32 + 23, j * 32 + 16) ^ k_9(j * 32 + 31, j * 32 + 24);
                    key_item_tmp(j * 32 + 15, j * 32 + 8) = k_9(j * 32 + 7, j * 32) ^ k_e(j * 32 + 15, j * 32 + 8) ^
                                                            k_b(j * 32 + 23, j * 32 + 16) ^
                                                            k_d(j * 32 + 31, j * 32 + 24);
                    key_item_tmp(j * 32 + 23, j * 32 + 16) = k_d(j * 32 + 7, j * 32) ^ k_9(j * 32 + 15, j * 32 + 8) ^
                                                             k_e(j * 32 + 23, j * 32 + 16) ^
                                                             k_b(j * 32 + 31, j * 32 + 24);
                    key_item_tmp(j * 32 + 31, j * 32 + 24) = k_b(j * 32 + 7, j * 32) ^ k_d(j * 32 + 15, j * 32 + 8) ^
                                                             k_9(j * 32 + 23, j * 32 + 16) ^
                                                             k_e(j * 32 + 31, j * 32 + 24);
                }

                key_list[i] = key_item_tmp;
            }
        }
    }

decrypt_block : {
#pragma HLS PIPELINE II = 1
    ap_uint<128> state = ciphertext ^ key_list[14];
decrypt_loop:
    for (ap_uint<4> rnd = 1; rnd < 15; rnd++) {
#pragma HLS unroll
#if !defined(__SYNTHESIS__) && XF_SECURITY_DECRYPT_DEBUG == 1
        std::cout << std::endl << std::hex << "is_start:" << state << std::endl;
#endif
        // inverse Shift Row
        ap_uint<8> tmp_1 = state(111, 104);
        state(111, 104) = state(79, 72);
        state(79, 72) = state(47, 40);
        state(47, 40) = state(15, 8);
        state(15, 8) = tmp_1;

        ap_uint<8> tmp_2_1, tmp_2_2;
        tmp_2_1 = state(119, 112);
        state(119, 112) = state(55, 48);
        state(55, 48) = tmp_2_1;
        tmp_2_2 = state(87, 80);
        state(87, 80) = state(23, 16);
        state(23, 16) = tmp_2_2;

        ap_uint<8> tmp_3 = state(31, 24);
        state(31, 24) = state(63, 56);
        state(63, 56) = state(95, 88);
        state(95, 88) = state(127, 120);
        state(127, 120) = tmp_3;

#if !defined(__SYNTHESIS__) && XF_SECURITY_DECRYPT_DEBUG == 1
        std::cout << std::hex << "is_row:" << state << std::endl;
#endif
        // inverse subbyte and mix column
        ap_uint<128> k_9, k_b, k_d, k_e;
        for (int j = 0; j < 16; j++) {
#pragma HLS unroll
            ap_uint<32> rk_tbox = gtbox[state(j * 8 + 7, j * 8)];
            k_9(j * 8 + 7, j * 8) = rk_tbox(15, 8);
            k_b(j * 8 + 7, j * 8) = rk_tbox(31, 24);
            k_d(j * 8 + 7, j * 8) = rk_tbox(23, 16);
            k_e(j * 8 + 7, j * 8) = rk_tbox(7, 0);
        }

        if (rnd < 14) {
            for (int j = 0; j < 4; j++) {
#pragma HLS unroll
                state(j * 32 + 7, j * 32) = k_e(j * 32 + 7, j * 32) ^ k_b(j * 32 + 15, j * 32 + 8) ^
                                            k_d(j * 32 + 23, j * 32 + 16) ^ k_9(j * 32 + 31, j * 32 + 24);
                state(j * 32 + 15, j * 32 + 8) = k_9(j * 32 + 7, j * 32) ^ k_e(j * 32 + 15, j * 32 + 8) ^
                                                 k_b(j * 32 + 23, j * 32 + 16) ^ k_d(j * 32 + 31, j * 32 + 24);
                state(j * 32 + 23, j * 32 + 16) = k_d(j * 32 + 7, j * 32) ^ k_9(j * 32 + 15, j * 32 + 8) ^
                                                  k_e(j * 32 + 23, j * 32 + 16) ^ k_b(j * 32 + 31, j * 32 + 24);
                state(j * 32 + 31, j * 32 + 24) = k_b(j * 32 + 7, j * 32) ^ k_d(j * 32 + 15, j * 32 + 8) ^
                                                  k_9(j * 32 + 23, j * 32 + 16) ^ k_e(j * 32 + 31, j * 32 + 24);
            }
        } else {
            ap_uint<128> state_copy = state;
            for (int j = 0; j < 16; j++) {
#pragma HLS unroll
                state(j * 8 + 7, j * 8) = isbox[state_copy(j * 8 + 7, j * 8)];
            }
        }

#if !defined(__SYNTHESIS__) && XF_SECURITY_DECRYPT_DEBUG == 1
        std::cout << std::hex << "is_mbox:" << state << std::endl;
#endif
        state ^= key_list[14 - rnd];
#if !defined(__SYNTHESIS__) && XF_SECURITY_DECRYPT_DEBUG == 1
        std::cout << std::hex << "is_last:" << state << std::endl;
#endif
    }
    plaintext = state;
    return;
} // decrypt block
} // decrypt_one_word function



const ap_uint<8> Rcon[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

static void processWordTemp(ap_uint<32>& round_tmp, int& nRcon) {
    // RotWord
    ap_uint<8> round_shift_tmp = round_tmp(7, 0);
    round_tmp >>= 8;
    round_tmp(31, 24) = round_shift_tmp(7, 0);

    // SubWord and xor Rcon
    round_tmp(7, 0) = sboxEnc[round_tmp(7, 0)] ^ Rcon[nRcon];
    round_tmp(15, 8) = sboxEnc[round_tmp(15, 8)];
    round_tmp(23, 16) = sboxEnc[round_tmp(23, 16)];
    round_tmp(31, 24) = sboxEnc[round_tmp(31, 24)];

    nRcon++;
}

template <>
class aesEnc<128> {
private:
    void processWordTemp(ap_uint<32>& round_tmp, int& nRcon) {
        #pragma HLS inline
        const ap_uint<8> Rcon[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};
        // RotWord
        ap_uint<8> round_shift_tmp = round_tmp(7, 0);
        round_tmp >>= 8;
        round_tmp(31, 24) = round_shift_tmp(7, 0);
        // SubWord and xor Rcon
        round_tmp(7, 0) = sboxEnc[round_tmp(7, 0)] ^ Rcon[nRcon];
        round_tmp(15, 8) = sboxEnc[round_tmp(15, 8)];
        round_tmp(23, 16) = sboxEnc[round_tmp(23, 16)];
        round_tmp(31, 24) = sboxEnc[round_tmp(31, 24)];
        nRcon++;
    }
public:
    ap_uint<128> key_list[11];

    aesEnc() {
#pragma HLS resource variable = sbox_mix_col_1 core = ROM_nP_LUTRAM
#pragma HLS resource variable = sboxEnc core = ROM_2P_BRAM
#pragma HLS ARRAY_PARTITION variable = key_list complete dim = 1
    }

    void updateKey(ap_uint<128> cipherkey) {
        #pragma HLS inline off
        int nRcon = 0;
        ap_uint<128> roundkey, roundkey_1;
        ap_uint<32> round_tmp;

        key_list[0] = cipherkey;
        roundkey_1 = cipherkey;
        for (ap_uint<4> round_counter = 1; round_counter <= 10; round_counter++) {
            #pragma HLS pipeline
            // Generate roundkey
            // round 1 - 0, 1, 2, 3
            // round 2 - 4, 5, 6, 7
            // round 3 - 8, 9, 10, 11
            round_tmp(31, 0) = roundkey_1(127, 96);
            processWordTemp(round_tmp, nRcon);

            // xor temp
            roundkey(31, 0) = roundkey_1(31, 0) ^ round_tmp;
            roundkey(63, 32) = roundkey_1(63, 32) ^ roundkey(31, 0); // w[i-Nk] ^ w[i-1]
            roundkey(95, 64) = roundkey_1(95, 64) ^ roundkey(63, 32);
            roundkey(127, 96) = roundkey_1(127, 96) ^ roundkey(95, 64);

            roundkey_1 = roundkey;
            key_list[round_counter] = roundkey;
        }
    }

    void process(ap_uint<128> plaintext, ap_uint<128> cipherkey, ap_uint<128>& ciphertext) {
        #pragma HLS inline off
        ap_uint<128> state, state_1, state_2, state_3;
        ap_uint<8> tmp_1, tmp_2_1, tmp_2_2, tmp_3;
        ap_uint<11> addr;
        ap_uint<16> mix_col_1_tmp;

        state = plaintext ^ key_list[0];

        for (ap_uint<4> round_counter = 1; round_counter <= 10; round_counter++) {
            // Same for 128, 192, and 256
            // ShiftRow
            tmp_1 = state(15, 8);
            state(15, 8) = state(47, 40);
            state(47, 40) = state(79, 72);
            state(79, 72) = state(111, 104);
            state(111, 104) = tmp_1;

            tmp_2_1 = state(23, 16);
            state(23, 16) = state(87, 80);
            state(87, 80) = tmp_2_1;

            tmp_2_2 = state(55, 48);
            state(55, 48) = state(119, 112);
            state(119, 112) = tmp_2_2;

            tmp_3 = state(127, 120);
            state(127, 120) = state(95, 88);
            state(95, 88) = state(63, 56);
            state(63, 56) = state(31, 24);
            state(31, 24) = tmp_3;

            // SubByte and MixColumn
            // for each byte in state
            for (int i = 0; i < 16; i++) {
    #pragma HLS unroll

                mix_col_1_tmp = sbox_mix_col_1[state(i * 8 + 7, i * 8)];
                state_1(i * 8 + 7, i * 8) = mix_col_1_tmp(7, 0);
                state_2(i * 8 + 7, i * 8) = mix_col_1_tmp(15, 8);
                state_3(i * 8 + 7, i * 8) = state_1(i * 8 + 7, i * 8) ^ state_2(i * 8 + 7, i * 8);
            }
            if (round_counter < 10) {
                for (int i = 0; i < 4; i++) {
    #pragma HLS unroll

                    state(i * 32 + 7, i * 32) = state_2(i * 32 + 7, i * 32) ^ state_3(i * 32 + 15, i * 32 + 8) ^
                                                state_1(i * 32 + 23, i * 32 + 16) ^ state_1(i * 32 + 31, i * 32 + 24);
                    state(i * 32 + 15, i * 32 + 8) = state_1(i * 32 + 7, i * 32) ^ state_2(i * 32 + 15, i * 32 + 8) ^
                                                     state_3(i * 32 + 23, i * 32 + 16) ^ state_1(i * 32 + 31, i * 32 + 24);
                    state(i * 32 + 23, i * 32 + 16) = state_1(i * 32 + 7, i * 32) ^ state_1(i * 32 + 15, i * 32 + 8) ^
                                                      state_2(i * 32 + 23, i * 32 + 16) ^ state_3(i * 32 + 31, i * 32 + 24);
                    state(i * 32 + 31, i * 32 + 24) = state_3(i * 32 + 7, i * 32) ^ state_1(i * 32 + 15, i * 32 + 8) ^
                                                      state_1(i * 32 + 23, i * 32 + 16) ^ state_2(i * 32 + 31, i * 32 + 24);
                }
            } else {
                state = state_1;
            }

            state ^= key_list[round_counter];
        }
        ciphertext = state;
    }
};


// AES with key size 128
static void aes128Encrypt(ap_uint<128> plaintext, ap_uint<128> cipherkey, ap_uint<128>& ciphertext) {
#pragma HLS PIPELINE II = 1
    ap_uint<128> state, state_1, state_2, state_3;
    ap_uint<128> roundkey, roundkey_1;
    ap_uint<32> round_tmp;
    ap_uint<8> round_shift_tmp;
    ap_uint<8> tmp_1, tmp_2_1, tmp_2_2, tmp_3;
    ap_uint<4> round_counter;
    ap_uint<11> addr;
    ap_uint<16> mix_col_1_tmp;
    int nRcon = 0;

    roundkey_1(127, 0) = cipherkey(127, 0);

    // state init and add roundkey[0]
    state = plaintext ^ cipherkey(127, 0);

    // Start 10 rounds of process
    for (round_counter = 1; round_counter <= 10; round_counter++) {
        // Generate roundkey
        // round 1 - 0, 1, 2, 3
        // round 2 - 4, 5, 6, 7
        // round 3 - 8, 9, 10, 11
        round_tmp(31, 0) = roundkey_1(127, 96);
        processWordTemp(round_tmp, nRcon);

        // xor temp
        roundkey(31, 0) = roundkey_1(31, 0) ^ round_tmp;
        roundkey(63, 32) = roundkey_1(63, 32) ^ roundkey(31, 0); // w[i-Nk] ^ w[i-1]
        roundkey(95, 64) = roundkey_1(95, 64) ^ roundkey(63, 32);
        roundkey(127, 96) = roundkey_1(127, 96) ^ roundkey(95, 64);

        roundkey_1 = roundkey;

        // Same for 128, 192, and 256
        // ShiftRow
        tmp_1 = state(15, 8);
        state(15, 8) = state(47, 40);
        state(47, 40) = state(79, 72);
        state(79, 72) = state(111, 104);
        state(111, 104) = tmp_1;

        tmp_2_1 = state(23, 16);
        state(23, 16) = state(87, 80);
        state(87, 80) = tmp_2_1;

        tmp_2_2 = state(55, 48);
        state(55, 48) = state(119, 112);
        state(119, 112) = tmp_2_2;

        tmp_3 = state(127, 120);
        state(127, 120) = state(95, 88);
        state(95, 88) = state(63, 56);
        state(63, 56) = state(31, 24);
        state(31, 24) = tmp_3;

        // SubByte and MixColumn
        // for each byte in state
        for (int i = 0; i < 16; i++) {
#pragma HLS unroll

            mix_col_1_tmp = sbox_mix_col_1[state(i * 8 + 7, i * 8)];
            state_1(i * 8 + 7, i * 8) = mix_col_1_tmp(7, 0);
            state_2(i * 8 + 7, i * 8) = mix_col_1_tmp(15, 8);
            state_3(i * 8 + 7, i * 8) = state_1(i * 8 + 7, i * 8) ^ state_2(i * 8 + 7, i * 8);
        }
        if (round_counter < 10) {
            for (int i = 0; i < 4; i++) {
#pragma HLS unroll

                state(i * 32 + 7, i * 32) = state_2(i * 32 + 7, i * 32) ^ state_3(i * 32 + 15, i * 32 + 8) ^
                                            state_1(i * 32 + 23, i * 32 + 16) ^ state_1(i * 32 + 31, i * 32 + 24);
                state(i * 32 + 15, i * 32 + 8) = state_1(i * 32 + 7, i * 32) ^ state_2(i * 32 + 15, i * 32 + 8) ^
                                                 state_3(i * 32 + 23, i * 32 + 16) ^ state_1(i * 32 + 31, i * 32 + 24);
                state(i * 32 + 23, i * 32 + 16) = state_1(i * 32 + 7, i * 32) ^ state_1(i * 32 + 15, i * 32 + 8) ^
                                                  state_2(i * 32 + 23, i * 32 + 16) ^ state_3(i * 32 + 31, i * 32 + 24);
                state(i * 32 + 31, i * 32 + 24) = state_3(i * 32 + 7, i * 32) ^ state_1(i * 32 + 15, i * 32 + 8) ^
                                                  state_1(i * 32 + 23, i * 32 + 16) ^ state_2(i * 32 + 31, i * 32 + 24);
            }
        } else {
            state = state_1;
        }

        state ^= roundkey;
    }
    ciphertext = state;
    return;
}

template <>
class aesEnc<192> {
private:
    void processWordTemp(ap_uint<32>& round_tmp, int& nRcon) {
        #pragma HLS inline
        const ap_uint<8> Rcon[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};
        // RotWord
        ap_uint<8> round_shift_tmp = round_tmp(7, 0);
        round_tmp >>= 8;
        round_tmp(31, 24) = round_shift_tmp(7, 0);
        // SubWord and xor Rcon
        round_tmp(7, 0) = sboxEnc[round_tmp(7, 0)] ^ Rcon[nRcon];
        round_tmp(15, 8) = sboxEnc[round_tmp(15, 8)];
        round_tmp(23, 16) = sboxEnc[round_tmp(23, 16)];
        round_tmp(31, 24) = sboxEnc[round_tmp(31, 24)];
        nRcon++;
    }
public:
	ap_uint<128> key_list[13];
	
	aesEnc() {
#pragma HLS resource variable = sbox_mix_col_1 core = ROM_nP_LUTRAM
#pragma HLS resource variable = sboxEnc core = ROM_2P_BRAM
#pragma HLS ARRAY_PARTITION variable = key_list complete dim = 1
	}
	
    void updateKey(ap_uint<192> cipherkey) {
        #pragma HLS inline off
		ap_uint<128> roundkey, roundkey_1, roundkey_2;
		ap_uint<32> preRoundkeyWords[6];
		ap_uint<32> round_tmp;
		int nRcon = 0;
		
		roundkey = cipherkey(127, 0);
		roundkey_1(127, 0) = cipherkey(127, 0);
        key_list[0] = cipherkey(127, 0);
		
		for (ap_uint<4> round_counter = 1; round_counter <= 12; round_counter++) {
            #pragma HLS pipeline
			// Generate roundkey
			// round 1 - 0, 1, 2, 3
			// round 2 - 4, 5, 6, 7
			// round 3 - 8, 9, 10, 11

			if (round_counter == 1) {
				roundkey(63, 0) = cipherkey(191, 128);

				round_tmp(31, 0) = cipherkey(191, 160);
				processWordTemp(round_tmp, nRcon);
				roundkey(95, 64) = roundkey_1(31, 0) ^ round_tmp;

				roundkey(127, 96) = roundkey_1(63, 32) ^ roundkey(95, 64); // w[i-Nk] ^ w[i-1]

			} else {
				preRoundkeyWords[0] = roundkey_2(95, 64);
				preRoundkeyWords[1] = roundkey_2(127, 96);
				preRoundkeyWords[2] = roundkey_1(31, 0);
				preRoundkeyWords[3] = roundkey_1(63, 32);
				preRoundkeyWords[4] = roundkey_1(95, 64);
				preRoundkeyWords[5] = roundkey_1(127, 96);

				if (round_counter == 4 || round_counter == 7 || round_counter == 10) {
					roundkey(31, 0) = preRoundkeyWords[0] ^ preRoundkeyWords[5];
					roundkey(63, 32) = preRoundkeyWords[1] ^ roundkey(31, 0);

					round_tmp(31, 0) = roundkey(63, 32);
					processWordTemp(round_tmp, nRcon);
					roundkey(95, 64) = preRoundkeyWords[2] ^ round_tmp;

					roundkey(127, 96) = preRoundkeyWords[3] ^ roundkey(95, 64);
				} else if (round_counter == 2 || round_counter == 5 || round_counter == 8 || round_counter == 11) {
					roundkey(31, 0) = preRoundkeyWords[0] ^ preRoundkeyWords[5];
					roundkey(63, 32) = preRoundkeyWords[1] ^ roundkey(31, 0);
					roundkey(95, 64) = preRoundkeyWords[2] ^ roundkey(63, 32);
					roundkey(127, 96) = preRoundkeyWords[3] ^ roundkey(95, 64);
				} else if (round_counter == 3 || round_counter == 6 || round_counter == 9 || round_counter == 12) {
					round_tmp(31, 0) = preRoundkeyWords[5];
					processWordTemp(round_tmp, nRcon);
					roundkey(31, 0) = preRoundkeyWords[0] ^ round_tmp;

					roundkey(63, 32) = preRoundkeyWords[1] ^ roundkey(31, 0);
					roundkey(95, 64) = preRoundkeyWords[2] ^ roundkey(63, 32);
					roundkey(127, 96) = preRoundkeyWords[3] ^ roundkey(95, 64);
				}
			}

			roundkey_2 = roundkey_1;
			roundkey_1 = roundkey;
			key_list[round_counter] = roundkey;
		}
	}
	
    void process(ap_uint<128> plaintext, ap_uint<192> cipherkey, ap_uint<128>& ciphertext) {
        #pragma HLS inline off
		ap_uint<128> state, state_1, state_2, state_3;
		ap_uint<8> tmp_1, tmp_2_1, tmp_2_2, tmp_3;
		ap_uint<16> mix_col_1_tmp;
		
		state = plaintext ^ key_list[0];
		for (ap_uint<4> round_counter = 1; round_counter <= 12; round_counter++) {

			// ShiftRow
			tmp_1 = state(15, 8);
			state(15, 8) = state(47, 40);
			state(47, 40) = state(79, 72);
			state(79, 72) = state(111, 104);
			state(111, 104) = tmp_1;

			tmp_2_1 = state(23, 16);
			state(23, 16) = state(87, 80);
			state(87, 80) = tmp_2_1;

			tmp_2_2 = state(55, 48);
			state(55, 48) = state(119, 112);
			state(119, 112) = tmp_2_2;

			tmp_3 = state(127, 120);
			state(127, 120) = state(95, 88);
			state(95, 88) = state(63, 56);
			state(63, 56) = state(31, 24);
			state(31, 24) = tmp_3;

			// SubByte and MixColumn
			// for each byte in state
			for (int i = 0; i < 16; i++) {
	#pragma HLS unroll

				mix_col_1_tmp = sbox_mix_col_1[state(i * 8 + 7, i * 8)];
				state_1(i * 8 + 7, i * 8) = mix_col_1_tmp(7, 0);
				state_2(i * 8 + 7, i * 8) = mix_col_1_tmp(15, 8);
				state_3(i * 8 + 7, i * 8) = state_1(i * 8 + 7, i * 8) ^ state_2(i * 8 + 7, i * 8);
			}
			if (round_counter < 12) {
				for (int i = 0; i < 4; i++) {
	#pragma HLS unroll

					state(i * 32 + 7, i * 32) = state_2(i * 32 + 7, i * 32) ^ state_3(i * 32 + 15, i * 32 + 8) ^
												state_1(i * 32 + 23, i * 32 + 16) ^ state_1(i * 32 + 31, i * 32 + 24);
					state(i * 32 + 15, i * 32 + 8) = state_1(i * 32 + 7, i * 32) ^ state_2(i * 32 + 15, i * 32 + 8) ^
													 state_3(i * 32 + 23, i * 32 + 16) ^ state_1(i * 32 + 31, i * 32 + 24);
					state(i * 32 + 23, i * 32 + 16) = state_1(i * 32 + 7, i * 32) ^ state_1(i * 32 + 15, i * 32 + 8) ^
													  state_2(i * 32 + 23, i * 32 + 16) ^ state_3(i * 32 + 31, i * 32 + 24);
					state(i * 32 + 31, i * 32 + 24) = state_3(i * 32 + 7, i * 32) ^ state_1(i * 32 + 15, i * 32 + 8) ^
													  state_1(i * 32 + 23, i * 32 + 16) ^ state_2(i * 32 + 31, i * 32 + 24);
				}
			} else {
				state = state_1;
			}

			state ^= key_list[round_counter];
		}
		ciphertext = state;
		return;
	}
};

// AES with key size 192
static void aes192Encrypt(ap_uint<128> plaintext, ap_uint<192> cipherkey, ap_uint<128>& ciphertext) {
#pragma HLS PIPELINE II = 1
    ap_uint<128> state, state_1, state_2, state_3;
    ap_uint<128> roundkey, roundkey_1, roundkey_2;
    ap_uint<32> roundkeyWords[4];
    ap_uint<32> preRoundkeyWords[6];
    ap_uint<32> round_tmp;
    ap_uint<8> round_shift_tmp;
    ap_uint<8> tmp_1, tmp_2_1, tmp_2_2, tmp_3;
    ap_uint<4> round_counter;
    ap_uint<11> addr;
    ap_uint<16> mix_col_1_tmp;

    // state init and add roundkey[0]
    roundkey = cipherkey(127, 0);
    state = plaintext ^ roundkey;

    roundkey_1(127, 0) = cipherkey(127, 0);

    int nRcon = 0;

    // Start 12 rounds of process
    for (round_counter = 1; round_counter <= 12; round_counter++) {
        // Generate roundkey
        // round 1 - 0, 1, 2, 3
        // round 2 - 4, 5, 6, 7
        // round 3 - 8, 9, 10, 11

        if (round_counter == 1) {
            roundkey(63, 0) = cipherkey(191, 128);

            round_tmp(31, 0) = cipherkey(191, 160);
            processWordTemp(round_tmp, nRcon);
            roundkey(95, 64) = roundkey_1(31, 0) ^ round_tmp;

            roundkey(127, 96) = roundkey_1(63, 32) ^ roundkey(95, 64); // w[i-Nk] ^ w[i-1]

        } else {
            preRoundkeyWords[0] = roundkey_2(95, 64);
            preRoundkeyWords[1] = roundkey_2(127, 96);
            preRoundkeyWords[2] = roundkey_1(31, 0);
            preRoundkeyWords[3] = roundkey_1(63, 32);
            preRoundkeyWords[4] = roundkey_1(95, 64);
            preRoundkeyWords[5] = roundkey_1(127, 96);

            if (round_counter == 4 || round_counter == 7 || round_counter == 10) {
                roundkey(31, 0) = preRoundkeyWords[0] ^ preRoundkeyWords[5];
                roundkey(63, 32) = preRoundkeyWords[1] ^ roundkey(31, 0);

                round_tmp(31, 0) = roundkey(63, 32);
                processWordTemp(round_tmp, nRcon);
                roundkey(95, 64) = preRoundkeyWords[2] ^ round_tmp;

                roundkey(127, 96) = preRoundkeyWords[3] ^ roundkey(95, 64);
            } else if (round_counter == 2 || round_counter == 5 || round_counter == 8 || round_counter == 11) {
                roundkey(31, 0) = preRoundkeyWords[0] ^ preRoundkeyWords[5];
                roundkey(63, 32) = preRoundkeyWords[1] ^ roundkey(31, 0);
                roundkey(95, 64) = preRoundkeyWords[2] ^ roundkey(63, 32);
                roundkey(127, 96) = preRoundkeyWords[3] ^ roundkey(95, 64);
            } else if (round_counter == 3 || round_counter == 6 || round_counter == 9 || round_counter == 12) {
                round_tmp(31, 0) = preRoundkeyWords[5];
                processWordTemp(round_tmp, nRcon);
                roundkey(31, 0) = preRoundkeyWords[0] ^ round_tmp;

                roundkey(63, 32) = preRoundkeyWords[1] ^ roundkey(31, 0);
                roundkey(95, 64) = preRoundkeyWords[2] ^ roundkey(63, 32);
                roundkey(127, 96) = preRoundkeyWords[3] ^ roundkey(95, 64);
            }
        }

        roundkey_2 = roundkey_1;
        roundkey_1 = roundkey;

        // ShiftRow
        tmp_1 = state(15, 8);
        state(15, 8) = state(47, 40);
        state(47, 40) = state(79, 72);
        state(79, 72) = state(111, 104);
        state(111, 104) = tmp_1;

        tmp_2_1 = state(23, 16);
        state(23, 16) = state(87, 80);
        state(87, 80) = tmp_2_1;

        tmp_2_2 = state(55, 48);
        state(55, 48) = state(119, 112);
        state(119, 112) = tmp_2_2;

        tmp_3 = state(127, 120);
        state(127, 120) = state(95, 88);
        state(95, 88) = state(63, 56);
        state(63, 56) = state(31, 24);
        state(31, 24) = tmp_3;

        // SubByte and MixColumn
        // for each byte in state
        for (int i = 0; i < 16; i++) {
#pragma HLS unroll

            mix_col_1_tmp = sbox_mix_col_1[state(i * 8 + 7, i * 8)];
            state_1(i * 8 + 7, i * 8) = mix_col_1_tmp(7, 0);
            state_2(i * 8 + 7, i * 8) = mix_col_1_tmp(15, 8);
            state_3(i * 8 + 7, i * 8) = state_1(i * 8 + 7, i * 8) ^ state_2(i * 8 + 7, i * 8);
        }
        if (round_counter < 12) {
            for (int i = 0; i < 4; i++) {
#pragma HLS unroll

                state(i * 32 + 7, i * 32) = state_2(i * 32 + 7, i * 32) ^ state_3(i * 32 + 15, i * 32 + 8) ^
                                            state_1(i * 32 + 23, i * 32 + 16) ^ state_1(i * 32 + 31, i * 32 + 24);
                state(i * 32 + 15, i * 32 + 8) = state_1(i * 32 + 7, i * 32) ^ state_2(i * 32 + 15, i * 32 + 8) ^
                                                 state_3(i * 32 + 23, i * 32 + 16) ^ state_1(i * 32 + 31, i * 32 + 24);
                state(i * 32 + 23, i * 32 + 16) = state_1(i * 32 + 7, i * 32) ^ state_1(i * 32 + 15, i * 32 + 8) ^
                                                  state_2(i * 32 + 23, i * 32 + 16) ^ state_3(i * 32 + 31, i * 32 + 24);
                state(i * 32 + 31, i * 32 + 24) = state_3(i * 32 + 7, i * 32) ^ state_1(i * 32 + 15, i * 32 + 8) ^
                                                  state_1(i * 32 + 23, i * 32 + 16) ^ state_2(i * 32 + 31, i * 32 + 24);
            }
        } else {
            state = state_1;
        }

        state ^= roundkey;
    }
    ciphertext = state;
    return;
}

template <>
class aesDec<128> {
private:
    void processWordTemp(ap_uint<32>& round_tmp, int& nRcon) {
        #pragma HLS inline
        const ap_uint<8> Rcon[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};
        // RotWord
        ap_uint<8> round_shift_tmp = round_tmp(7, 0);
        round_tmp >>= 8;
        round_tmp(31, 24) = round_shift_tmp(7, 0);
        // SubWord and xor Rcon
        round_tmp(7, 0) = sboxEnc[round_tmp(7, 0)] ^ Rcon[nRcon];
        round_tmp(15, 8) = sboxEnc[round_tmp(15, 8)];
        round_tmp(23, 16) = sboxEnc[round_tmp(23, 16)];
        round_tmp(31, 24) = sboxEnc[round_tmp(31, 24)];
        nRcon++;
    }	
public:
	ap_uint<128> key_list[11];
	const int NR = 10;
	
	aesDec() {
#pragma HLS resource variable = isbox core = ROM_nP_LUTRAM
#pragma HLS resource variable = gtbox core = ROM_nP_LUTRAM
#pragma HLS resource variable = sbox core = ROM_2P_BRAM
#pragma HLS resource variable = sboxEnc core = ROM_2P_BRAM
#pragma HLS ARRAY_PARTITION variable = key_list complete dim = 1
	}
	
	void updateKey(ap_uint<128> cipherkey) {
        #pragma HLS inline off
		ap_uint<128> roundkey_1, roundkey_2, roundkey_tmp;
		ap_uint<32> round_tmp;
		ap_uint<11> addr;
		int nRcon = 0;
		
		key_list[0] = cipherkey(127, 0);
        roundkey_1(127, 0) = cipherkey(127, 0);
		
		for (ap_uint<4> i = 1; i <= NR; i++) {
#pragma HLS unroll
            // std::cout << "Iter " << i << "\n";

            // round 1 - 0, 1, 2, 3
            // round 2 - 4, 5, 6, 7
            // round 3 - 8, 9, 10, 11
            round_tmp(31, 0) = roundkey_1(127, 96);
            processWordTemp(round_tmp, nRcon);

            // xor temp
            roundkey_tmp(31, 0) = roundkey_1(31, 0) ^ round_tmp;
            roundkey_tmp(63, 32) = roundkey_1(63, 32) ^ roundkey_tmp(31, 0); // w[i-Nk] ^ w[i-1]
            roundkey_tmp(95, 64) = roundkey_1(95, 64) ^ roundkey_tmp(63, 32);
            roundkey_tmp(127, 96) = roundkey_1(127, 96) ^ roundkey_tmp(95, 64);

            roundkey_1 = roundkey_tmp;

            if (i == NR) {
                key_list[i] = roundkey_tmp;
            } else { // inverse mix column
                ap_uint<128> k_9, k_b, k_d, k_e;
                for (int j = 0; j < 16; j++) {
#pragma HLS unroll
                    ap_uint<8> rk_sbox2 = sbox[roundkey_tmp(j * 8 + 7, j * 8)];
                    ap_uint<32> rk_tbox = gtbox[rk_sbox2];
                    k_9(j * 8 + 7, j * 8) = rk_tbox(15, 8);
                    k_b(j * 8 + 7, j * 8) = rk_tbox(31, 24);
                    k_d(j * 8 + 7, j * 8) = rk_tbox(23, 16);
                    k_e(j * 8 + 7, j * 8) = rk_tbox(7, 0);
                }

                ap_uint<128> key_item_tmp;
                for (int j = 0; j < 4; j++) {
#pragma HLS unroll
                    key_item_tmp(j * 32 + 7, j * 32) = k_e(j * 32 + 7, j * 32) ^ k_b(j * 32 + 15, j * 32 + 8) ^
                                                       k_d(j * 32 + 23, j * 32 + 16) ^ k_9(j * 32 + 31, j * 32 + 24);
                    key_item_tmp(j * 32 + 15, j * 32 + 8) = k_9(j * 32 + 7, j * 32) ^ k_e(j * 32 + 15, j * 32 + 8) ^
                                                            k_b(j * 32 + 23, j * 32 + 16) ^
                                                            k_d(j * 32 + 31, j * 32 + 24);
                    key_item_tmp(j * 32 + 23, j * 32 + 16) = k_d(j * 32 + 7, j * 32) ^ k_9(j * 32 + 15, j * 32 + 8) ^
                                                             k_e(j * 32 + 23, j * 32 + 16) ^
                                                             k_b(j * 32 + 31, j * 32 + 24);
                    key_item_tmp(j * 32 + 31, j * 32 + 24) = k_b(j * 32 + 7, j * 32) ^ k_d(j * 32 + 15, j * 32 + 8) ^
                                                             k_9(j * 32 + 23, j * 32 + 16) ^
                                                             k_e(j * 32 + 31, j * 32 + 24);
                }

                key_list[i] = key_item_tmp;
            }
        }
	}
	
    void process(ap_uint<128> ciphertext, ap_uint<128> cipherkey, ap_uint<128>& plaintext) {
        #pragma HLS inline off
		ap_uint<128> state = ciphertext ^ key_list[NR];
	decrypt_loop:
		for (ap_uint<4> rnd = 1; rnd <= NR; rnd++) {
	#pragma HLS unroll

			// Inverse Shift Row
			ap_uint<8> tmp_1 = state(111, 104);
			state(111, 104) = state(79, 72);
			state(79, 72) = state(47, 40);
			state(47, 40) = state(15, 8);
			state(15, 8) = tmp_1;

			ap_uint<8> tmp_2_1, tmp_2_2;
			tmp_2_1 = state(119, 112);
			state(119, 112) = state(55, 48);
			state(55, 48) = tmp_2_1;
			tmp_2_2 = state(87, 80);
			state(87, 80) = state(23, 16);
			state(23, 16) = tmp_2_2;

			ap_uint<8> tmp_3 = state(31, 24);
			state(31, 24) = state(63, 56);
			state(63, 56) = state(95, 88);
			state(95, 88) = state(127, 120);
			state(127, 120) = tmp_3;

			// Inverse SubByte and MixColumn
			ap_uint<128> k_9, k_b, k_d, k_e;
			for (int j = 0; j < 16; j++) {
	#pragma HLS unroll
				ap_uint<32> rk_tbox = gtbox[state(j * 8 + 7, j * 8)];
				k_9(j * 8 + 7, j * 8) = rk_tbox(15, 8);
				k_b(j * 8 + 7, j * 8) = rk_tbox(31, 24);
				k_d(j * 8 + 7, j * 8) = rk_tbox(23, 16);
				k_e(j * 8 + 7, j * 8) = rk_tbox(7, 0);
			}

			if (rnd < NR) {
				for (int j = 0; j < 4; j++) {
	#pragma HLS unroll
					state(j * 32 + 7, j * 32) = k_e(j * 32 + 7, j * 32) ^ k_b(j * 32 + 15, j * 32 + 8) ^
												k_d(j * 32 + 23, j * 32 + 16) ^ k_9(j * 32 + 31, j * 32 + 24);
					state(j * 32 + 15, j * 32 + 8) = k_9(j * 32 + 7, j * 32) ^ k_e(j * 32 + 15, j * 32 + 8) ^
													 k_b(j * 32 + 23, j * 32 + 16) ^ k_d(j * 32 + 31, j * 32 + 24);
					state(j * 32 + 23, j * 32 + 16) = k_d(j * 32 + 7, j * 32) ^ k_9(j * 32 + 15, j * 32 + 8) ^
													  k_e(j * 32 + 23, j * 32 + 16) ^ k_b(j * 32 + 31, j * 32 + 24);
					state(j * 32 + 31, j * 32 + 24) = k_b(j * 32 + 7, j * 32) ^ k_d(j * 32 + 15, j * 32 + 8) ^
													  k_9(j * 32 + 23, j * 32 + 16) ^ k_e(j * 32 + 31, j * 32 + 24);
				}
			} else {
				ap_uint<128> state_copy = state;
				for (int j = 0; j < 16; j++) {
	#pragma HLS unroll
					state(j * 8 + 7, j * 8) = isbox[state_copy(j * 8 + 7, j * 8)];
				}
			}

			// Key addition
			state ^= key_list[NR - rnd];
		}
		plaintext = state;
		return;		
	}
};

/**
 * @brief aes128Decrypt is the basic function for decrypting one block
with one cipher key
 *
 * @param ciphertext input one word cipher text to be decrypted, 128 bits.
 * @param plainkey input plain key, 128 bits.
 * @param plaintext output one word encrypted text, 128 bit.
 *
 */
static void aes128Decrypt(ap_uint<128> ciphertext, ap_uint<128> cipherkey, ap_uint<128>& plaintext) {
    const int NR = 10; // total round number

    static ap_uint<128> key_list[11]; // subkey list for 1 + 10 rounds
#pragma HLS ARRAY_PARTITION variable = key_list complete dim = 1
    static bool is_first = true;
    static bool update_key_en;
    static ap_uint<256> ckey;

    ap_uint<128> roundkey_1, roundkey_2, roundkey_tmp;
    ap_uint<32> round_tmp;
    ap_uint<11> addr;
    int nRcon = 0;

    // if key requires be updated
    if (ckey != cipherkey)
        update_key_en = true;
    else
        update_key_en = false;

    // Generate all other keys
    if (update_key_en || is_first) {
        is_first = false;
        ckey = cipherkey;

        key_list[0] = cipherkey(127, 0);

        roundkey_1(127, 0) = cipherkey(127, 0);

    key_gen_loop:
        // NR rounds
        for (ap_uint<4> i = 1; i <= NR; i++) {
#pragma HLS unroll
            // std::cout << "Iter " << i << "\n";

            // round 1 - 0, 1, 2, 3
            // round 2 - 4, 5, 6, 7
            // round 3 - 8, 9, 10, 11
            round_tmp(31, 0) = roundkey_1(127, 96);
            processWordTemp(round_tmp, nRcon);

            // xor temp
            roundkey_tmp(31, 0) = roundkey_1(31, 0) ^ round_tmp;
            roundkey_tmp(63, 32) = roundkey_1(63, 32) ^ roundkey_tmp(31, 0); // w[i-Nk] ^ w[i-1]
            roundkey_tmp(95, 64) = roundkey_1(95, 64) ^ roundkey_tmp(63, 32);
            roundkey_tmp(127, 96) = roundkey_1(127, 96) ^ roundkey_tmp(95, 64);

            roundkey_1 = roundkey_tmp;

            if (i == NR) {
                key_list[i] = roundkey_tmp;
            } else { // inverse mix column
                ap_uint<128> k_9, k_b, k_d, k_e;
                for (int j = 0; j < 16; j++) {
#pragma HLS unroll
                    ap_uint<8> rk_sbox2 = sbox[roundkey_tmp(j * 8 + 7, j * 8)];
                    ap_uint<32> rk_tbox = gtbox[rk_sbox2];
                    k_9(j * 8 + 7, j * 8) = rk_tbox(15, 8);
                    k_b(j * 8 + 7, j * 8) = rk_tbox(31, 24);
                    k_d(j * 8 + 7, j * 8) = rk_tbox(23, 16);
                    k_e(j * 8 + 7, j * 8) = rk_tbox(7, 0);
                }

                ap_uint<128> key_item_tmp;
                for (int j = 0; j < 4; j++) {
#pragma HLS unroll
                    key_item_tmp(j * 32 + 7, j * 32) = k_e(j * 32 + 7, j * 32) ^ k_b(j * 32 + 15, j * 32 + 8) ^
                                                       k_d(j * 32 + 23, j * 32 + 16) ^ k_9(j * 32 + 31, j * 32 + 24);
                    key_item_tmp(j * 32 + 15, j * 32 + 8) = k_9(j * 32 + 7, j * 32) ^ k_e(j * 32 + 15, j * 32 + 8) ^
                                                            k_b(j * 32 + 23, j * 32 + 16) ^
                                                            k_d(j * 32 + 31, j * 32 + 24);
                    key_item_tmp(j * 32 + 23, j * 32 + 16) = k_d(j * 32 + 7, j * 32) ^ k_9(j * 32 + 15, j * 32 + 8) ^
                                                             k_e(j * 32 + 23, j * 32 + 16) ^
                                                             k_b(j * 32 + 31, j * 32 + 24);
                    key_item_tmp(j * 32 + 31, j * 32 + 24) = k_b(j * 32 + 7, j * 32) ^ k_d(j * 32 + 15, j * 32 + 8) ^
                                                             k_9(j * 32 + 23, j * 32 + 16) ^
                                                             k_e(j * 32 + 31, j * 32 + 24);
                }

                key_list[i] = key_item_tmp;
            }
        }
    }

decrypt_block : {
#pragma HLS PIPELINE II = 1
    ap_uint<128> state = ciphertext ^ key_list[NR];
decrypt_loop:
    for (ap_uint<4> rnd = 1; rnd <= NR; rnd++) {
#pragma HLS unroll

        // Inverse Shift Row
        ap_uint<8> tmp_1 = state(111, 104);
        state(111, 104) = state(79, 72);
        state(79, 72) = state(47, 40);
        state(47, 40) = state(15, 8);
        state(15, 8) = tmp_1;

        ap_uint<8> tmp_2_1, tmp_2_2;
        tmp_2_1 = state(119, 112);
        state(119, 112) = state(55, 48);
        state(55, 48) = tmp_2_1;
        tmp_2_2 = state(87, 80);
        state(87, 80) = state(23, 16);
        state(23, 16) = tmp_2_2;

        ap_uint<8> tmp_3 = state(31, 24);
        state(31, 24) = state(63, 56);
        state(63, 56) = state(95, 88);
        state(95, 88) = state(127, 120);
        state(127, 120) = tmp_3;

        // Inverse SubByte and MixColumn
        ap_uint<128> k_9, k_b, k_d, k_e;
        for (int j = 0; j < 16; j++) {
#pragma HLS unroll
            ap_uint<32> rk_tbox = gtbox[state(j * 8 + 7, j * 8)];
            k_9(j * 8 + 7, j * 8) = rk_tbox(15, 8);
            k_b(j * 8 + 7, j * 8) = rk_tbox(31, 24);
            k_d(j * 8 + 7, j * 8) = rk_tbox(23, 16);
            k_e(j * 8 + 7, j * 8) = rk_tbox(7, 0);
        }

        if (rnd < NR) {
            for (int j = 0; j < 4; j++) {
#pragma HLS unroll
                state(j * 32 + 7, j * 32) = k_e(j * 32 + 7, j * 32) ^ k_b(j * 32 + 15, j * 32 + 8) ^
                                            k_d(j * 32 + 23, j * 32 + 16) ^ k_9(j * 32 + 31, j * 32 + 24);
                state(j * 32 + 15, j * 32 + 8) = k_9(j * 32 + 7, j * 32) ^ k_e(j * 32 + 15, j * 32 + 8) ^
                                                 k_b(j * 32 + 23, j * 32 + 16) ^ k_d(j * 32 + 31, j * 32 + 24);
                state(j * 32 + 23, j * 32 + 16) = k_d(j * 32 + 7, j * 32) ^ k_9(j * 32 + 15, j * 32 + 8) ^
                                                  k_e(j * 32 + 23, j * 32 + 16) ^ k_b(j * 32 + 31, j * 32 + 24);
                state(j * 32 + 31, j * 32 + 24) = k_b(j * 32 + 7, j * 32) ^ k_d(j * 32 + 15, j * 32 + 8) ^
                                                  k_9(j * 32 + 23, j * 32 + 16) ^ k_e(j * 32 + 31, j * 32 + 24);
            }
        } else {
            ap_uint<128> state_copy = state;
            for (int j = 0; j < 16; j++) {
#pragma HLS unroll
                state(j * 8 + 7, j * 8) = isbox[state_copy(j * 8 + 7, j * 8)];
            }
        }

        // Key addition
        state ^= key_list[NR - rnd];
    }
    plaintext = state;
    return;
} // decrypt block
} // decrypt_one_word function


template <>
class aesDec<192> {
private:
    void processWordTemp(ap_uint<32>& round_tmp, int& nRcon) {
        #pragma HLS inline
        const ap_uint<8> Rcon[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};
        // RotWord
        ap_uint<8> round_shift_tmp = round_tmp(7, 0);
        round_tmp >>= 8;
        round_tmp(31, 24) = round_shift_tmp(7, 0);
        // SubWord and xor Rcon
        round_tmp(7, 0) = sboxEnc[round_tmp(7, 0)] ^ Rcon[nRcon];
        round_tmp(15, 8) = sboxEnc[round_tmp(15, 8)];
        round_tmp(23, 16) = sboxEnc[round_tmp(23, 16)];
        round_tmp(31, 24) = sboxEnc[round_tmp(31, 24)];
        nRcon++;
    }	
public:
	ap_uint<128> key_list[13];
	const int NR = 12;
	
    aesDec() {
#pragma HLS resource variable = isbox core = ROM_nP_LUTRAM
#pragma HLS resource variable = gtbox core = ROM_nP_LUTRAM
#pragma HLS resource variable = sbox core = ROM_2P_BRAM
#pragma HLS resource variable = sboxEnc core = ROM_2P_BRAM
#pragma HLS ARRAY_PARTITION variable = key_list complete dim = 1
    }
	
    void updateKey(ap_uint<192> cipherkey) {
		#pragma HLS inline off
		ap_uint<128> roundkey_1, roundkey_2, roundkey_tmp;
		ap_uint<32> round_tmp;
		ap_uint<32> preRoundkeyWords[6];
		int nRcon = 0;
		key_list[0] = cipherkey(127, 0);
        roundkey_1(127, 0) = cipherkey(127, 0);
		
        for (ap_uint<4> i = 1; i <= NR; i++) {
#pragma HLS unroll
            // std::cout << "Iter " << i << "\n";

            // Generate roundkey
            // round 0 - 0, 1, 2, 3
            // round 1 - 4, 5, 6, 7
            // round 2 - 8, 9, 10, 11

            if (i == 1) {
                roundkey_tmp(63, 0) = cipherkey(191, 128);

                round_tmp(31, 0) = cipherkey(191, 160);
                processWordTemp(round_tmp, nRcon);
                roundkey_tmp(95, 64) = roundkey_1(31, 0) ^ round_tmp;

                roundkey_tmp(127, 96) = roundkey_1(63, 32) ^ roundkey_tmp(95, 64); // w[i-Nk] ^ w[i-1]

            } else {
                preRoundkeyWords[0] = roundkey_2(95, 64);
                preRoundkeyWords[1] = roundkey_2(127, 96);
                preRoundkeyWords[2] = roundkey_1(31, 0);
                preRoundkeyWords[3] = roundkey_1(63, 32);
                preRoundkeyWords[4] = roundkey_1(95, 64);
                preRoundkeyWords[5] = roundkey_1(127, 96);

                if (i == 4 || i == 7 || i == 10) {
                    roundkey_tmp(31, 0) = preRoundkeyWords[0] ^ preRoundkeyWords[5];
                    roundkey_tmp(63, 32) = preRoundkeyWords[1] ^ roundkey_tmp(31, 0);

                    round_tmp(31, 0) = roundkey_tmp(63, 32);
                    processWordTemp(round_tmp, nRcon);
                    roundkey_tmp(95, 64) = preRoundkeyWords[2] ^ round_tmp;

                    roundkey_tmp(127, 96) = preRoundkeyWords[3] ^ roundkey_tmp(95, 64);
                } else if (i == 2 || i == 5 || i == 8 || i == 11) {
                    roundkey_tmp(31, 0) = preRoundkeyWords[0] ^ preRoundkeyWords[5];
                    roundkey_tmp(63, 32) = preRoundkeyWords[1] ^ roundkey_tmp(31, 0);
                    roundkey_tmp(95, 64) = preRoundkeyWords[2] ^ roundkey_tmp(63, 32);
                    roundkey_tmp(127, 96) = preRoundkeyWords[3] ^ roundkey_tmp(95, 64);
                } else if (i == 3 || i == 6 || i == 9 || i == 12) {
                    round_tmp(31, 0) = preRoundkeyWords[5];
                    processWordTemp(round_tmp, nRcon);
                    roundkey_tmp(31, 0) = preRoundkeyWords[0] ^ round_tmp;

                    roundkey_tmp(63, 32) = preRoundkeyWords[1] ^ roundkey_tmp(31, 0);
                    roundkey_tmp(95, 64) = preRoundkeyWords[2] ^ roundkey_tmp(63, 32);
                    roundkey_tmp(127, 96) = preRoundkeyWords[3] ^ roundkey_tmp(95, 64);
                }
            }

            roundkey_2 = roundkey_1;
            roundkey_1 = roundkey_tmp;

            if (i == NR) {
                key_list[i] = roundkey_tmp;
            } else { // inverse mix column
                ap_uint<128> k_9, k_b, k_d, k_e;
                for (int j = 0; j < 16; j++) {
#pragma HLS unroll
                    ap_uint<8> rk_sbox2 = sbox[roundkey_tmp(j * 8 + 7, j * 8)];
                    ap_uint<32> rk_tbox = gtbox[rk_sbox2];
                    k_9(j * 8 + 7, j * 8) = rk_tbox(15, 8);
                    k_b(j * 8 + 7, j * 8) = rk_tbox(31, 24);
                    k_d(j * 8 + 7, j * 8) = rk_tbox(23, 16);
                    k_e(j * 8 + 7, j * 8) = rk_tbox(7, 0);
                }

                ap_uint<128> key_item_tmp;
                for (int j = 0; j < 4; j++) {
#pragma HLS unroll
                    key_item_tmp(j * 32 + 7, j * 32) = k_e(j * 32 + 7, j * 32) ^ k_b(j * 32 + 15, j * 32 + 8) ^
                                                       k_d(j * 32 + 23, j * 32 + 16) ^ k_9(j * 32 + 31, j * 32 + 24);
                    key_item_tmp(j * 32 + 15, j * 32 + 8) = k_9(j * 32 + 7, j * 32) ^ k_e(j * 32 + 15, j * 32 + 8) ^
                                                            k_b(j * 32 + 23, j * 32 + 16) ^
                                                            k_d(j * 32 + 31, j * 32 + 24);
                    key_item_tmp(j * 32 + 23, j * 32 + 16) = k_d(j * 32 + 7, j * 32) ^ k_9(j * 32 + 15, j * 32 + 8) ^
                                                             k_e(j * 32 + 23, j * 32 + 16) ^
                                                             k_b(j * 32 + 31, j * 32 + 24);
                    key_item_tmp(j * 32 + 31, j * 32 + 24) = k_b(j * 32 + 7, j * 32) ^ k_d(j * 32 + 15, j * 32 + 8) ^
                                                             k_9(j * 32 + 23, j * 32 + 16) ^
                                                             k_e(j * 32 + 31, j * 32 + 24);
                }

                key_list[i] = key_item_tmp;
            }
        }
	}
    
	void process(ap_uint<128> ciphertext, ap_uint<192> cipherkey, ap_uint<128>& plaintext) {
		#pragma HLS inline off
		ap_uint<128> state = ciphertext ^ key_list[NR];
	decrypt_loop:
		for (ap_uint<4> rnd = 1; rnd <= NR; rnd++) {
	#pragma HLS unroll

			// Inverse Shift Row
			ap_uint<8> tmp_1 = state(111, 104);
			state(111, 104) = state(79, 72);
			state(79, 72) = state(47, 40);
			state(47, 40) = state(15, 8);
			state(15, 8) = tmp_1;

			ap_uint<8> tmp_2_1, tmp_2_2;
			tmp_2_1 = state(119, 112);
			state(119, 112) = state(55, 48);
			state(55, 48) = tmp_2_1;
			tmp_2_2 = state(87, 80);
			state(87, 80) = state(23, 16);
			state(23, 16) = tmp_2_2;

			ap_uint<8> tmp_3 = state(31, 24);
			state(31, 24) = state(63, 56);
			state(63, 56) = state(95, 88);
			state(95, 88) = state(127, 120);
			state(127, 120) = tmp_3;

			// Inverse SubByte and MixColumn
			ap_uint<128> k_9, k_b, k_d, k_e;
			for (int j = 0; j < 16; j++) {
	#pragma HLS unroll
				ap_uint<32> rk_tbox = gtbox[state(j * 8 + 7, j * 8)];
				k_9(j * 8 + 7, j * 8) = rk_tbox(15, 8);
				k_b(j * 8 + 7, j * 8) = rk_tbox(31, 24);
				k_d(j * 8 + 7, j * 8) = rk_tbox(23, 16);
				k_e(j * 8 + 7, j * 8) = rk_tbox(7, 0);
			}

			if (rnd < NR) {
				for (int j = 0; j < 4; j++) {
	#pragma HLS unroll
					state(j * 32 + 7, j * 32) = k_e(j * 32 + 7, j * 32) ^ k_b(j * 32 + 15, j * 32 + 8) ^
												k_d(j * 32 + 23, j * 32 + 16) ^ k_9(j * 32 + 31, j * 32 + 24);
					state(j * 32 + 15, j * 32 + 8) = k_9(j * 32 + 7, j * 32) ^ k_e(j * 32 + 15, j * 32 + 8) ^
													 k_b(j * 32 + 23, j * 32 + 16) ^ k_d(j * 32 + 31, j * 32 + 24);
					state(j * 32 + 23, j * 32 + 16) = k_d(j * 32 + 7, j * 32) ^ k_9(j * 32 + 15, j * 32 + 8) ^
													  k_e(j * 32 + 23, j * 32 + 16) ^ k_b(j * 32 + 31, j * 32 + 24);
					state(j * 32 + 31, j * 32 + 24) = k_b(j * 32 + 7, j * 32) ^ k_d(j * 32 + 15, j * 32 + 8) ^
													  k_9(j * 32 + 23, j * 32 + 16) ^ k_e(j * 32 + 31, j * 32 + 24);
				}
			} else {
				ap_uint<128> state_copy = state;
				for (int j = 0; j < 16; j++) {
	#pragma HLS unroll
					state(j * 8 + 7, j * 8) = isbox[state_copy(j * 8 + 7, j * 8)];
				}
			}

			// Key addition
			state ^= key_list[NR - rnd];
		}
		plaintext = state;
		return;		
	}
};

/**
 * @brief aes192Decrypt is the basic function for decrypting one block
with one 192-bit cipher key
 *
 * @param ciphertext input one word cipher text to be decrypted, 128 bits.
 * @param plainkey input plain key, 192 bits.
 * @param plaintext output one word encrypted text, 128 bit.
 *
 */
static void aes192Decrypt(ap_uint<128> ciphertext, ap_uint<192> cipherkey, ap_uint<128>& plaintext) {
    const int NR = 12; // total round number

    static ap_uint<128> key_list[NR + 1]; // subkey list for 1 + NR rounds
#pragma HLS ARRAY_PARTITION variable = key_list complete dim = 1
    static bool is_first = true;
    static bool update_key_en;
    static ap_uint<256> ckey;

    ap_uint<128> roundkey_1, roundkey_2, roundkey_tmp;
    ap_uint<32> round_tmp;
    ap_uint<32> roundkeyWords[4];
    ap_uint<32> preRoundkeyWords[6];
    ap_uint<11> addr;
    int nRcon = 0;

    // if key requires be updated
    if (ckey != cipherkey)
        update_key_en = true;
    else
        update_key_en = false;

    // Generate all other keys
    if (update_key_en || is_first) {
        is_first = false;
        ckey = cipherkey;

        key_list[0] = cipherkey(127, 0);

        roundkey_1(127, 0) = cipherkey(127, 0);

    key_gen_loop:
        // NR rounds
        for (ap_uint<4> i = 1; i <= NR; i++) {
#pragma HLS unroll
            // std::cout << "Iter " << i << "\n";

            // Generate roundkey
            // round 0 - 0, 1, 2, 3
            // round 1 - 4, 5, 6, 7
            // round 2 - 8, 9, 10, 11

            if (i == 1) {
                roundkey_tmp(63, 0) = cipherkey(191, 128);

                round_tmp(31, 0) = cipherkey(191, 160);
                processWordTemp(round_tmp, nRcon);
                roundkey_tmp(95, 64) = roundkey_1(31, 0) ^ round_tmp;

                roundkey_tmp(127, 96) = roundkey_1(63, 32) ^ roundkey_tmp(95, 64); // w[i-Nk] ^ w[i-1]

            } else {
                preRoundkeyWords[0] = roundkey_2(95, 64);
                preRoundkeyWords[1] = roundkey_2(127, 96);
                preRoundkeyWords[2] = roundkey_1(31, 0);
                preRoundkeyWords[3] = roundkey_1(63, 32);
                preRoundkeyWords[4] = roundkey_1(95, 64);
                preRoundkeyWords[5] = roundkey_1(127, 96);

                if (i == 4 || i == 7 || i == 10) {
                    roundkey_tmp(31, 0) = preRoundkeyWords[0] ^ preRoundkeyWords[5];
                    roundkey_tmp(63, 32) = preRoundkeyWords[1] ^ roundkey_tmp(31, 0);

                    round_tmp(31, 0) = roundkey_tmp(63, 32);
                    processWordTemp(round_tmp, nRcon);
                    roundkey_tmp(95, 64) = preRoundkeyWords[2] ^ round_tmp;

                    roundkey_tmp(127, 96) = preRoundkeyWords[3] ^ roundkey_tmp(95, 64);
                } else if (i == 2 || i == 5 || i == 8 || i == 11) {
                    roundkey_tmp(31, 0) = preRoundkeyWords[0] ^ preRoundkeyWords[5];
                    roundkey_tmp(63, 32) = preRoundkeyWords[1] ^ roundkey_tmp(31, 0);
                    roundkey_tmp(95, 64) = preRoundkeyWords[2] ^ roundkey_tmp(63, 32);
                    roundkey_tmp(127, 96) = preRoundkeyWords[3] ^ roundkey_tmp(95, 64);
                } else if (i == 3 || i == 6 || i == 9 || i == 12) {
                    round_tmp(31, 0) = preRoundkeyWords[5];
                    processWordTemp(round_tmp, nRcon);
                    roundkey_tmp(31, 0) = preRoundkeyWords[0] ^ round_tmp;

                    roundkey_tmp(63, 32) = preRoundkeyWords[1] ^ roundkey_tmp(31, 0);
                    roundkey_tmp(95, 64) = preRoundkeyWords[2] ^ roundkey_tmp(63, 32);
                    roundkey_tmp(127, 96) = preRoundkeyWords[3] ^ roundkey_tmp(95, 64);
                }
            }

            roundkey_2 = roundkey_1;
            roundkey_1 = roundkey_tmp;

            if (i == NR) {
                key_list[i] = roundkey_tmp;
            } else { // inverse mix column
                ap_uint<128> k_9, k_b, k_d, k_e;
                for (int j = 0; j < 16; j++) {
#pragma HLS unroll
                    ap_uint<8> rk_sbox2 = sbox[roundkey_tmp(j * 8 + 7, j * 8)];
                    ap_uint<32> rk_tbox = gtbox[rk_sbox2];
                    k_9(j * 8 + 7, j * 8) = rk_tbox(15, 8);
                    k_b(j * 8 + 7, j * 8) = rk_tbox(31, 24);
                    k_d(j * 8 + 7, j * 8) = rk_tbox(23, 16);
                    k_e(j * 8 + 7, j * 8) = rk_tbox(7, 0);
                }

                ap_uint<128> key_item_tmp;
                for (int j = 0; j < 4; j++) {
#pragma HLS unroll
                    key_item_tmp(j * 32 + 7, j * 32) = k_e(j * 32 + 7, j * 32) ^ k_b(j * 32 + 15, j * 32 + 8) ^
                                                       k_d(j * 32 + 23, j * 32 + 16) ^ k_9(j * 32 + 31, j * 32 + 24);
                    key_item_tmp(j * 32 + 15, j * 32 + 8) = k_9(j * 32 + 7, j * 32) ^ k_e(j * 32 + 15, j * 32 + 8) ^
                                                            k_b(j * 32 + 23, j * 32 + 16) ^
                                                            k_d(j * 32 + 31, j * 32 + 24);
                    key_item_tmp(j * 32 + 23, j * 32 + 16) = k_d(j * 32 + 7, j * 32) ^ k_9(j * 32 + 15, j * 32 + 8) ^
                                                             k_e(j * 32 + 23, j * 32 + 16) ^
                                                             k_b(j * 32 + 31, j * 32 + 24);
                    key_item_tmp(j * 32 + 31, j * 32 + 24) = k_b(j * 32 + 7, j * 32) ^ k_d(j * 32 + 15, j * 32 + 8) ^
                                                             k_9(j * 32 + 23, j * 32 + 16) ^
                                                             k_e(j * 32 + 31, j * 32 + 24);
                }

                key_list[i] = key_item_tmp;
            }
        }
    }

decrypt_block : {
#pragma HLS PIPELINE II = 1
    ap_uint<128> state = ciphertext ^ key_list[NR];
decrypt_loop:
    for (ap_uint<4> rnd = 1; rnd <= NR; rnd++) {
#pragma HLS unroll

        // Inverse Shift Row
        ap_uint<8> tmp_1 = state(111, 104);
        state(111, 104) = state(79, 72);
        state(79, 72) = state(47, 40);
        state(47, 40) = state(15, 8);
        state(15, 8) = tmp_1;

        ap_uint<8> tmp_2_1, tmp_2_2;
        tmp_2_1 = state(119, 112);
        state(119, 112) = state(55, 48);
        state(55, 48) = tmp_2_1;
        tmp_2_2 = state(87, 80);
        state(87, 80) = state(23, 16);
        state(23, 16) = tmp_2_2;

        ap_uint<8> tmp_3 = state(31, 24);
        state(31, 24) = state(63, 56);
        state(63, 56) = state(95, 88);
        state(95, 88) = state(127, 120);
        state(127, 120) = tmp_3;

        // Inverse SubByte and MixColumn
        ap_uint<128> k_9, k_b, k_d, k_e;
        for (int j = 0; j < 16; j++) {
#pragma HLS unroll
            ap_uint<32> rk_tbox = gtbox[state(j * 8 + 7, j * 8)];
            k_9(j * 8 + 7, j * 8) = rk_tbox(15, 8);
            k_b(j * 8 + 7, j * 8) = rk_tbox(31, 24);
            k_d(j * 8 + 7, j * 8) = rk_tbox(23, 16);
            k_e(j * 8 + 7, j * 8) = rk_tbox(7, 0);
        }

        if (rnd < NR) {
            for (int j = 0; j < 4; j++) {
#pragma HLS unroll
                state(j * 32 + 7, j * 32) = k_e(j * 32 + 7, j * 32) ^ k_b(j * 32 + 15, j * 32 + 8) ^
                                            k_d(j * 32 + 23, j * 32 + 16) ^ k_9(j * 32 + 31, j * 32 + 24);
                state(j * 32 + 15, j * 32 + 8) = k_9(j * 32 + 7, j * 32) ^ k_e(j * 32 + 15, j * 32 + 8) ^
                                                 k_b(j * 32 + 23, j * 32 + 16) ^ k_d(j * 32 + 31, j * 32 + 24);
                state(j * 32 + 23, j * 32 + 16) = k_d(j * 32 + 7, j * 32) ^ k_9(j * 32 + 15, j * 32 + 8) ^
                                                  k_e(j * 32 + 23, j * 32 + 16) ^ k_b(j * 32 + 31, j * 32 + 24);
                state(j * 32 + 31, j * 32 + 24) = k_b(j * 32 + 7, j * 32) ^ k_d(j * 32 + 15, j * 32 + 8) ^
                                                  k_9(j * 32 + 23, j * 32 + 16) ^ k_e(j * 32 + 31, j * 32 + 24);
            }
        } else {
            ap_uint<128> state_copy = state;
            for (int j = 0; j < 16; j++) {
#pragma HLS unroll
                state(j * 8 + 7, j * 8) = isbox[state_copy(j * 8 + 7, j * 8)];
            }
        }

        // Key addition
        state ^= key_list[NR - rnd];
    }
    plaintext = state;
    return;
} // decrypt block
} // decrypt_one_word function

namespace internal {

/**
 * @brief aesEncrypt is the basic function for ciphering one block
with one cipher key
 *
 * @tparam W cipher key width
 * @param plaintext input one word plain text to be encrypted, 128 bits.
 * @param cipherkey input cipher key, W bits.
 * @param ciphertext output one word encrypted text, 128 bit.
 *
 */
template <int W>
static void aesEncrypt(ap_uint<128> plaintext, ap_uint<W> cipherkey, ap_uint<128>& ciphertext) {
    if (W == 128) {
        aes128Encrypt(plaintext, cipherkey, ciphertext);
    } else if (W == 192) {
        aes192Encrypt(plaintext, cipherkey, ciphertext);
    } else if (W == 256) {
        aes256Encrypt(plaintext, cipherkey, ciphertext);
    }
}

/**
 * @brief aesDecrypt is the basic function for decrypting one block
with one cipher key
 *
 * @tparam W cipher key width
 * @param ciphertext input one word cipher text to be decrypted, 128 bits.
 * @param plainkey input plain key, W bits.
 * @param plaintext output one word encrypted text, 128 bit.
 *
 */
template <int W>
static void aesDecrypt(ap_uint<128> ciphertext, ap_uint<W> cipherkey, ap_uint<128>& plaintext) {
    if (W == 128) {
        aes128Decrypt(ciphertext, cipherkey, plaintext);
    } else if (W == 192) {
        aes192Decrypt(ciphertext, cipherkey, plaintext);
    } else if (W == 256) {
        aes256Decrypt(ciphertext, cipherkey, plaintext);
    }
}

} // internal namespace

} // namespace security
} // namespace xf
#endif
