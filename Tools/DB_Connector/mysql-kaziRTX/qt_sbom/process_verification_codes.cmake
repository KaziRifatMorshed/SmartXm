
list(REMOVE_DUPLICATES QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES)
# Go through each package that has verification codes (a code for each file that is part of a
# package), sort them, concatenate them, and calculate the sha1.
# Prepend the value with the PackageVerificationCode: prefix, so it can be directly evaluated
# in the spdx.in file via configure_file.
foreach(_sbom_package IN LISTS QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES)
    set(_codes ${QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES_${_sbom_package}})
    list(SORT _codes)
    string(REPLACE ";" "" _codes "${_codes}")
    string(SHA1 _verification_code "${_codes}")
    set(QT_SBOM_VERIFICATION_CODE_${_sbom_package} "
PackageVerificationCode: ${_verification_code}")
endforeach()
unset(_sbom_package)
unset(_codes)
unset(_verification_code)
