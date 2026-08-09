function platform_pre_acpi(){
	cp Platform/Xiaomi/sdm845/AcpiTables/beryllium/panel-tianma-fts.asl Platform/Xiaomi/sdm845/AcpiTables/beryllium/panel.asl
	DEVICE="beryllium"
	EXT="-tianma-fts"
	GEN_ACPI=true
}