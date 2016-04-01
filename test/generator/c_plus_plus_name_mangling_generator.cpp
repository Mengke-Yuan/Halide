#include "Halide.h"

// TODO: Add HalideExtern support for C++ mangling, hopefully using automatic argument type deduction
Halide::Expr extract_value_global(Halide::Expr arg) {
    return Halide::Internal::Call::make(Halide::type_of<int>(),
                                        "extract_value_global", {arg},
                                        Halide::Internal::Call::ExternCPlusPlus);
}

Halide::Expr extract_value_ns(Halide::Expr arg) {
    return Halide::Internal::Call::make(Halide::type_of<int>(),
                                        "HalideTest::extract_value_ns", {arg},
                                        Halide::Internal::Call::ExternCPlusPlus);
}

class CPlusPlusNameManglingGenerator : public Halide::Generator<CPlusPlusNameManglingGenerator> {
public:
    // Use all the parameter types to make sure mangling works for each of them.
    // TODO: verify this provides full coverage.
    ImageParam input{UInt(8), 1, "input"};
    Param<int8_t> offset_i8{"offset_i8", 0};
    Param<uint8_t> offset_u8{"offset_u8", 0};
    Param<int16_t> offset_i16{"offset_i16", 0};
    Param<uint16_t> offset_u16{"offset_u16", 0};
    Param<int32_t> offset_i32{"offset_i32", 0};
    Param<uint32_t> offset_u32{"offset_u32", 0};
    Param<int64_t> offset_i64{"offset_i64", 0};
    Param<uint64_t> offset_u64{"offset_u64", 0};

    Param<bool> scale_direction{"scale_direction", 1};
    Param<float> scale_f{"scale_f", 0};
    Param<double> scale_d{"scale_d", 0};
    Param<int32_t *> ptr{"ptr", 0};
    Param<int32_t const *> const_ptr{"const_ptr", 0};

    Func build() {
        assert(get_target().has_feature(Target::CPlusPlusMangling));
        Var x("x");
        Func f("f");

        Expr offset = offset_i8 + offset_u8 + offset_i16 + offset_u16 +
                      offset_i32 + offset_u32 + offset_i64 + offset_u64 +
                      extract_value_global(ptr) + extract_value_ns(const_ptr);

        // No significance to the calculation here.
        f(x) = select(scale_direction, (input(x) * scale_f + offset) / scale_d,
                                       (input(x) * scale_d + offset) / scale_f);

        return f;
    }
};

Halide::RegisterGenerator<CPlusPlusNameManglingGenerator> register_my_gen{"c_plus_plus_name_mangling"};