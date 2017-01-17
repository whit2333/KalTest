

SET( hybrid_input_dirs bp gen it kern tpc vtx )

FOREACH( _input_dir ${hybrid_input_dirs} )
    LIST( APPEND ROOT_DICT_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/${_input_dir} )
ENDFOREACH()


FOREACH( _input_dir ${hybrid_input_dirs} )

    AUX_SOURCE_DIRECTORY( ${_input_dir} hybrid_sources )
    FILE( GLOB headers "${_input_dir}/*.h")
    FILE( GLOB linkdef "${_input_dir}/LinkDef.h")
    list(REMOVE_ITEM headers ${linkdef} )

    #PREPARE_ROOT_DICT_HEADERS( ${_input_dir} )

    #GEN_ROOT_DICT_SOURCES( ${_input_dir}Dict.cxx )
  ROOT_GENERATE_DICTIONARY(${_input_dir} ${headers} LINKDEF ${linkdef})

  #AUX_SOURCE_DIRECTORY( ${_input_dir} sources )

    LIST( APPEND hybrid_sources ${ROOT_DICT_OUTPUT_SOURCES} )

ENDFOREACH()

INCLUDE_DIRECTORIES( ./main )
INCLUDE_DIRECTORIES( ${ROOT_DICT_INCLUDE_DIRS} )
INCLUDE_DIRECTORIES( BEFORE ${ROOT_INCLUDE_DIRS} )

AUX_SOURCE_DIRECTORY( ./main hybrid_sources )

ADD_KALTEST_EXAMPLE( hybrid ${hybrid_sources} )

