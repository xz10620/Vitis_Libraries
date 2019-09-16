.. CompressionLib_Docs documentation master file, created by
   sphinx-quickstart on Thu Jun 20 14:04:09 2019.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

.. _l2_demo:

====
Demo
====

Demo examples for lz4 and snappy kernels are present in **L2/demo/** directory.

Before building any of the examples, following commands need to be executed:

.. code-block:: bash
   
   $ cd L2/
   $ source ./env.csh
   $ source <Vitis_Installed_Path>/installs/lin64/Vitis/2019.2/settings64.csh
   $ source <Vitis_Installed_Path>/xbb/xrt/packages/setenv.csh
   $ cd ./demo/ 

By executing above commands, we initialize the environment for build. Device used is U200, which can be modified in ``env.csh`` script file in ``L2/`` directory.


LZ4 Compression and Decompression
---------------------------------

Lz4 example resides in ``demo/lz4`` directory. To compile and test run this example execute the following commands:

.. code-block:: bash
   
   $ cd ./lz4
   $ make run TARGET=sw_emu

Variable ``TARGET`` can take the following values:

	- **sw_emu**	: software emulation
	
	- **hw_emu**	: hardware emulation
	
	- **hw**	: run on actual hardware

The binary host file generated is named as "**xil_lz4_8b**", using `PARALLEL_BLOCK` value of 8 (default), and present in ``./build`` directory.
Following is the usage of the executable:

1. To execute single file for compression 	: ``./xil_lz4_8b -cx <compress xclbin> -c <file_name>``

2. To execute single file for decompression	: ``./xil_lz4_8b -dx <decompress xclbin> -d <file_name.lz4>``

3. To validate various files together		: ``./xil_lz4_8b -cx <compress xclbin> -dx <decompress xclbin> -l <files.list>``
	
	- ``<files.list>``: Contains various file names with current path

The usage of the generated executable is as follows:

.. code-block:: bash
   
   Usage: application.exe -[-h-c-l-d-B-x]
        --help,             -h      Print Help Options   Default: [false]
	--compress_xclbin   -cx     Compress binary
        --compress,         -c      Compress
        --file_list,        -l      List of Input Files
        --decompress_xclbin -dx     Decompress binary
        --decompress,       -d      Decompress
        --block_size,       -B      Compress Block Size [0-64: 1-256: 2-1024: 3-4096] Default: [0]
        --flow,             -x      Validation [0-All: 1-XcXd: 2-XcSd: 3-ScXd] Default: [1]


Snappy Compression and Decompression
------------------------------------

Snappy example resides in ``demo/snappy`` directory. To compile and test run this example execute the following commands:

.. code-block:: bash
   
   $ cd ./snappy
   $ make run TARGET=sw_emu

Variable ``TARGET`` can take the following values:

	- **sw_emu**	: software emulation
	
	- **hw_emu**	: hardware emulation
	
	- **hw**	: run on actual hardware

The binary host file generated is named as "**xil_snappy_8b**", using `PARALLEL_BLOCK` value of 8 (default), and present in ``./build`` directory.
Following is the usage of the executable:

1. To execute single file for compression 	: ``./xil_snappy_8b -cx <compress xclbin> -c <file_name>``

2. To execute single file for decompression	: ``./xil_snappy_8b -dx <decompress xclbin> -d <file_name.snappy>``

3. To validate various files together		: ``./xil_snappy_8b -cx <compress xclbin> -dx <decompress xclbin> -l <files.list>``
	
	- ``<files.list>``: Contains various file names with current path

The usage of the generated executable is as follows:

.. code-block:: bash
   
   Usage: application.exe -[-h-c-l-d-B-x]
        --help,             -h      Print Help Options   Default: [false]
        --compress,         -c      Compress
	--compress_xclbin   -cx     Compress binary
        --file_list,        -l      List of Input Files
	--decompress_xclbin -dx     Decompress binary
        --decompress,       -d      Decompress
        --block_size,       -B      Compress Block Size [0-64: 1-256: 2-1024: 3-4096] Default: [0]
        --flow,             -x      Validation [0-All: 1-XcXd: 2-XcSd: 3-ScXd] Default: [1]

LZ4-Streaming Compression and Decompression
-------------------------------------------

LZ4-Streaming example resides in ``demo/lz4_streaming`` directory. To compile and test run this example execute the following commands:

.. code-block:: bash
   
   $ cd ./lz4_streaming
   $ make run TARGET=sw_emu

Variable ``TARGET`` can take the following values:

	- **sw_emu**	: software emulation
	
	- **hw_emu**	: hardware emulation
	
	- **hw**	: run on actual hardware

The binary host file generated is named as "**xil_lz4_streaming**", using `PARALLEL_BLOCK` value of 8 (default), and present in ``./build`` directory.
Following is the usage of the executable:

1. To execute single file for compression 	: ``./xil_lz4_streaming -cx <compress xclbin> -c <file_name>``

2. To execute single file for decompression	: ``./xil_lz4_streaming -dx <decompress xclbin> -d <file_name.snappy>``

3. To validate various files together		: ``./xil_lz4_streaming -cx <compress xclbin> -dx <decompress xclbin> -l <files.list>``
	
	- ``<files.list>``: Contains various file names with current path

The usage of the generated executable is as follows:

.. code-block:: bash
   
   Usage: application.exe -[-h-c-l-d-B-x]
        --help,             -h      Print Help Options   Default: [false]
        --compress,         -c      Compress
	--compress_xclbin   -cx     Compress binary
        --file_list,        -l      List of Input Files
	--decompress_xclbin -dx     Decompress binary
        --decompress,       -d      Decompress
        --block_size,       -B      Compress Block Size [0-64: 1-256: 2-1024: 3-4096] Default: [0]
        --flow,             -x      Validation [0-All: 1-XcXd: 2-XcSd: 3-ScXd] Default: [1]


LZ4-P2P-Decompress Decompression
-------------------------------------------

LZ4-P2P-Decompress example resides in ``demo/lz4_p2p_decompress`` directory. To compile and test run this example execute the following commands:

.. code-block:: bash
   
   $ cd ./lz4_p2p_decompress
   $ make run TARGET=sw_emu

Variable ``TARGET`` can take the following values:

	- **sw_emu**	: software emulation
	
	- **hw_emu**	: hardware emulation
	
	- **hw**	: run on actual hardware

The binary host file generated is named as "**xil_lz4_8b**", using `PARALLEL_BLOCK` value of 8 (default), and present in ``./build`` directory.
Following is the usage of the executable:

1. To execute single file for decompression	          : ``./xil_lz4_8b -dx <decompress xclbin> -d <file_name.snappy>``

3. To execute various files together for decompression    : ``./xil_lz4_8b -cx <compress xclbin> -dx <decompress xclbin> -l <files.list>``
	
	- ``<files.list>``: Contains various file names with current path

The usage of the generated executable is as follows:

.. code-block:: bash
   
   Usage: application.exe -[-h-c-l-d-B-x]
        --help,             -h      Print Help Options   Default: [false]
        --file_list,        -l      List of Input Files
	--decompress_xclbin -dx     Decompress binary
        --decompress,       -d      Decompress
        --block_size,       -B      Compress Block Size [0-64: 1-256: 2-1024: 3-4096] Default: [0]
        --flow,             -x      Validation [0-All: 1-XcXd: 2-XcSd: 3-ScXd] Default: [1]
