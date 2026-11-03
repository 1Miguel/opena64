find_program(SREC_CAT "srec_cat" REQUIRED)

# @brief Merge two binaries at specified offsets.
#
# @param TARGET_BIN_1 first target object.
# @param TARGET_BIN_2 second binary file.
# @param OFFSET_2     second target offset.
#
# @output Outputs a ${TARGET_OBJ}.bin file.
macro(srec_cat_1 TARGET_OBJ TARGET_BIN_2 OFFSET_2)
  get_filename_component(SECOND_BIN_PATH
    "${TARGET_BIN_2}" ABSOLUTE
    BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

  set(MERGED_BIN
    "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_OBJ}_merged.bin")

  add_custom_command(TARGET ${TARGET_OBJ} POST_BUILD
    COMMAND "${SREC_CAT}"
      "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_OBJ}.bin" -Binary
        -crop 0x00 "${OFFSET_2}" -offset 0x00
      "${SECOND_BIN_PATH}" -Binary -offset "${OFFSET_2}"
      -o "${MERGED_BIN}" -Binary
    BYPRODUCTS "${MERGED_BIN}"
    COMMENT "Merging binaries for ${TARGET_OBJ}"
    VERBATIM
  )
endmacro()
