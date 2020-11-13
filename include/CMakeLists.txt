# We install all headers to the header directory, except for those that start with an underscore.
# These are internal, not meant to be exposed to the ARM->ART conversion process, and are not
# included by any "normal" header files.

install (
	DIRECTORY
		${art_internal_include_dir}
	DESTINATION
		${art_header_directory_location}
	PATTERN "_*" EXCLUDE
	PATTERN "*.txt" EXCLUDE
	PATTERN ".svn" EXCLUDE
	PATTERN ".DS_Store" EXCLUDE
	)
