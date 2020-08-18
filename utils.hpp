namespace si {

  template <typename ... Types>
  struct types_holder { };

  template <int N, typename... T>
  struct tuple_element;

  template <typename T0, typename... T>
  struct tuple_element<0, T0, T...> {
    typedef T0 type;
  };
  template <int N, typename T0, typename... T>
  struct tuple_element<N, T0, T...> {
    typedef typename tuple_element<N-1, T...>::type type;
  };
}
