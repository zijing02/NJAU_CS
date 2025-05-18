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
			<SelfTest>OnUse</SelfTest>
		</StartupConfig>
	</Runtime>
	<Signature URI="#ccme_error_info" Algorithm="FIPS140_INTEGRITY">MD0CHQC1hMvdJ7l7R28dB8J5ujatWuvDeJMQtHT/7FTLAhxlK8/tYgcRmZdOj1YZe1EJOa4mY2TMjwLJBF3L</Signature>
	<Signature URI="#ccme_aux_entropy" Algorithm="FIPS140_INTEGRITY">MD0CHH9wDMnXi6VUuw4Q0PzUZsChc5Y7Q14EskdbO+gCHQCD+K47nPTjh7GkyTEIUDmnw88gxkUdWcsmH9A0</Signature>
	<Signature URI="#ccme_base" Algorithm="FIPS140_INTEGRITY">MDwCHB57fNuM6BSbR3Iw++B+KiGrfwWpTkwJbaLmUUsCHAlMkbgBnyDxM3zUu4QT4u2EQertXu8qU6pK7Xs=</Signature>
	<Signature URI="#ccme_asym" Algorithm="FIPS140_INTEGRITY">MDwCHFNSqrY2S7ETl1jwJHuCoxoejJiSPN/3zl7Os54CHB6AsZSrp9Prq0okLG4owmMfmsw5iC9hP+98FHQ=</Signature>
	<Signature URI="#ccme_ecc_accel_fips" Algorithm="FIPS140_INTEGRITY">MD4CHQC6bec2NFQPW+eBUtcfyxamaNDEAW7rD5KDgr8IAh0AsQabce87SVBS6V0kU8CV5n3l+qE3cwNFiDIkBg==</Signature>
	<Signature URI="#ccme_ecc" Algorithm="FIPS140_INTEGRITY">MDwCHFT15mi5RjZxu+U6HL6kj5kE23HFXdk2Gxt1H5YCHEItdimsBn6dm+6homMBDejJCM1+9gJAeB4Y/k8=</Signature>
	<Signature URI="#ccme_base_non_fips" Algorithm="FIPS140_INTEGRITY">MD0CHA0nOQHHWXtkp3/ZSa1Kexu7EOTy+7Y+ZfyYCA4CHQCXbh0XhaieElcSTDKpoMlxLumzmSibTyZGd6xd</Signature>
	<Signature URI="#ccme_ecc_accel_non_fips" Algorithm="FIPS140_INTEGRITY">MDwCHEdefgZ81J/KbcCv3B4728sq5ZrDB2USLygxhkQCHHh4vgni7wQw+3Bh+z5l831PMyMCGpvRSDODVRI=</Signature>
	<Signature URI="#ccme_ecc_non_fips" Algorithm="FIPS140_INTEGRITY">MDwCHFxaqMjuhAbrxdQ3pGuJ3No9xvedXziIcda6aAUCHFHihLyyt4y7GDRSi7Ay42Fegtur2JUheISq3Js=</Signature>
	<Signature URI="#master" Algorithm="FIPS140_INTEGRITY">MD4CHQCz7gqjaW6LPETXYoYHKGKMBK/D5ygtlSSpk5fCAh0AsBZjJBe1JYzPDdocQmqvkQkdbbeDcWeIqGOPLA==</Signature>
	<Signature URI="#Crypto-C ME" Algorithm="FIPS140_INTEGRITY">MD0CHDI20gweEWxJa4NNom8Gi1+gNqfrZ1Z+L/w8pkkCHQCYwdNof+0Td1NH+rIMagGmytK9vyRxmmHmhwuL</Signature>
	<Signature URI="#runtime" Algorithm="FIPS140_INTEGRITY">MDwCHG0oQRCfOb6Tu9nO57lKwx2lvfSffa5MSJZsNu8CHE3zpCigmJDLjOXwzWfWSmLvSD1SjoRTnr+CO30=</Signature>
</Configuration>

