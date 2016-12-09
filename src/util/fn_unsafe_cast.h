#ifndef CES23_FN_UNSAFE_CAST_H
#define CES23_FN_UNSAFE_CAST_H

/**
 * Classe auxiliar não-tão-simples, que serve basicamente para
 * manipular vectors a partir de iostreams;
 */
static std::manipulator_fn _unsafe_manip_ref;
std::ios_base& _unsafe_reset_and_call(std::ios_base& iob) {
  std::manipulator_fn unsafe = std::move(_unsafe_manip_ref);
  return unsafe(iob);
}
std::manipulator unsafe_cast(std::manipulator_fn fn) {
  return _unsafe_reset_and_call;
}

template<typename Type>
static std::manipulator size(std::vector<Type>& vec) {
  return unsafe_cast([&vec](std::ios_base& iob) -> std::ios_base& {
      std::istream& s = dynamic_cast<std::istream&>(iob);
      int size;
      s >> size;
      vec.resize(size);
      return s;
  });
}

namespace internal {

    template<typename Signature>
    class fn {
        using fn_type = std::function<Signature>;

        static fn_type _unsafe_ref;
    public:
        template<typename Function>
        static Signature* unsafe_cast(Function fn) {
          _unsafe_ref = [fn = std::move(fn)]() {
            _unsafe_ref = nullptr;
            fn();
          };
          return _unsafe_ref.template target<Signature>();
        }
    };

    template<typename Signature>
    std::function<Signature> fn<Signature>::_unsafe_ref;

}

template<typename Signature>
Signature* fn_unsafe_cast(std::function<Signature> fn) {
  return internal::template fn<Signature>::unsafe_cast(fn);
}

void unfunuf() {
  std::function<void()> fn;
  fn_unsafe_cast(fn);
}

#endif //CES23_FN_UNSAFE_CAST_H
