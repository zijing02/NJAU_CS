<?rsa version="1.0" encoding="utf-8"?>
<Configuration>
	<Product Id="Crypto-C ME">
		<Version>RSA BSAFE Crypto-C ME 4.1.2.0 2015-11-30 15:09</Version>
		<ReleaseDate>2015-11-30 15:09</ReleaseDate>
		<ExpDate>""</ExpDate>
		<Copyright>
			Copyright (C) RSA
		</Copyright>
		<Library Id="master">cryptocme</Library>
	</Product>
	<Runtime Id="runtime">
		<LoadOrder>
			<Library Id="ccme_error_info">ccme_error_info</Library>
			<Library Id="ccme_aux_entropy">ccme_aux_entropy</Library>
			<Library Id="ccme_base">ccme_base</Library>
			<Library Id="ccme_asym">ccme_asym</Library>
			<Library Id="ccme_ecc_accel_fips">ccme_ecc_accel_fips</Library>
			<Library Id="ccme_ecc">ccme_ecc</Library>
			<Library Id="ccme_base_non_fips">ccme_base_non_fips</Library>
			<Library Id="ccme_ecc_accel_non_fips">ccme_ecc_accel_non_fips</Library>
			<Library Id="ccme_ecc_non_fips">ccme_ecc_non_fips</Library>
		</LoadOrder>
		<StartupConfig>
			<SelfTest>OnLoad</SelfTest>
		</StartupConfig>
	</Runtime>
	<Signature URI="#ccme_error_info" Algorithm="FIPS140_INTEGRITY">MDwCHACeUCfioZIBVs34bG92a/mks9BmgpD31H6ckDYCHA/8odviedGw0aK+1GcNi4cZtvMhYfqUEbqBp70=</Signature>
	<Signature URI="#ccme_aux_entropy" Algorithm="FIPS140_INTEGRITY">MD0CHDUdGW1CmzROG41aEagmJu/tpDahH9R8Ig0NaSwCHQCp0erZdEHlz6Hw2UDK/wkod3vPO1gWJPnJLakv</Signature>
	<Signature URI="#ccme_base" Algorithm="FIPS140_INTEGRITY">MD4CHQCrQUg1P2t07DT0Nao0AIeRb0tCXHEn5qpxAnHWAh0AhF9YtUJk2+XiXyEbc5LrZJKHT/0ljX3qAsF2qw==</Signature>
	<Signature URI="#ccme_asym" Algorithm="FIPS140_INTEGRITY">MDwCHAqYF4W0BBy3Mj0oHS18a/GuNYkx7WGRjXTIhzkCHB3aVJystqvBBOl6BLeTn4jP7j0y3YCfGLVbSJo=</Signature>
	<Signature URI="#ccme_ecc_accel_fips" Algorithm="FIPS140_INTEGRITY">MDwCHGsu9POGTKq8MBvqQH8Aaiv15ZU7YkPbjgejKbICHEzbLI49mTRULHIOJWyA1FRtG2q8YB7Tcf0l/XQ=</Signature>
	<Signature URI="#ccme_ecc" Algorithm="FIPS140_INTEGRITY">MD0CHDX5knA7xWoAhRjQOBK/BqLMBrpoyAq0k1y6t2wCHQC5PpaVQsXfoOzm0pYi2zxfY/6MPo+eHZClbZPE</Signature>
	<Signature URI="#ccme_base_non_fips" Algorithm="FIPS140_INTEGRITY">MD0CHQClygZVeiD5ADSVxJH2DFXi77MaRCcqogGwa0hGAhxH5ZcsPpjibjs0BDDmbbF4C5PH4r11uuJ7D8hm</Signature>
	<Signature URI="#ccme_ecc_accel_non_fips" Algorithm="FIPS140_INTEGRITY">MD0CHQCbB3Ncg2vU8fk/nPAuOi81L9m8eNnjQsJjNmHlAhwnPWsTWwOrUjkF1StslM5oeyrIb/Jryi3TXKxD</Signature>
	<Signature URI="#ccme_ecc_non_fips" Algorithm="FIPS140_INTEGRITY">MDwCHCBtQBntxQe6wAXK4vrRwRR7HiDKTPkoVavTx9YCHAILTGZXSKWwc6thM5hOwR9T9ZC4RM5mSXVxZkE=</Signature>
	<Signature URI="#master" Algorithm="FIPS140_INTEGRITY">MDwCHHRIsiUo6hbEjjm4Yxl53jLZA6IhxpwCi4+TY/MCHFSdm1gKxNQ9iXrR8hsck4l3sbCkFbnHAgwGxos=</Signature>
	<Signature URI="#Crypto-C ME" Algorithm="FIPS140_INTEGRITY">MD0CHCwizu0q2NeRJTluoZqqhOSc6Ixwl4oKWf7A5/0CHQCWy9vyc5uVb8/zxJn+zmswvHtJEc+zLptVcUJO</Signature>
	<Signature URI="#runtime" Algorithm="FIPS140_INTEGRITY">MD0CHQCs6UJaaY5HpMvqCka9bJt6XNyrhmWGVGWY6E6cAhwOsrCGzERaEK1p65DD0lHEM3Riu0F7MAiO34ZZ</Signature>
</Configuration>

