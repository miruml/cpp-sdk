# development example
function(build_miru_development_example)
    add_executable(development_example.out examples/development/main.cpp)
    target_link_libraries(development_example.out PRIVATE miru)
endfunction()

# production example
function(build_miru_production_example)
    add_executable(production_example.out examples/production/main.cpp)
    target_link_libraries(production_example.out PRIVATE miru)
endfunction()

function(build_miru_examples)
    build_miru_development_example()
    build_miru_production_example()
endfunction()