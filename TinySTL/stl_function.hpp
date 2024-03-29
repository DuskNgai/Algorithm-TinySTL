#ifndef _TINYSTL_FUNCTION_HPP_
#define _TINYSTL_FUNCTION_HPP_

namespace TinySTL {

    //! After C++11, lambda function together with type deduction have taken over these functionalities...
    //! That is, they all have been deprecated.

    // Two base functor.

    template <typename Arg, typename Return>
    struct unary_function {
        using argument_type = Arg;
        using return_type   = Return;
    };

    template <typename Arg1, typename Arg2, typename Return>
    struct binary_function {
        using first_argument_type  = Arg1;
        using second_argument_type = Arg2;
        using return_type          = Return;
    };

    //! ----- Relational ----- !//

    template <typename T>
    struct equal_to : public binary_function<T, T, bool> {
        bool operator()(const T& a, const T& b) const {
            return a == b;
        }
    };

    template <typename T>
    struct not_equal_to : public binary_function<T, T, bool> {
        bool operator()(const T& a, const T& b) const {
            return a != b;
        }
    };

    template <typename T>
    struct less : public binary_function<T, T, bool> {
        bool operator()(const T& a, const T& b) const {
            return a < b;
        }
    };

    template <typename T>
    struct less_equal : public binary_function<T, T, bool> {
        bool operator()(const T& a, const T& b) const {
            return a <= b;
        }
    };

    template <typename T>
    struct greater : public binary_function<T, T, bool> {
        bool operator()(const T& a, const T& b) const {
            return a > b;
        }
    };

    template <typename T>
    struct greater_equal : public binary_function<T, T, bool> {
        bool operator()(const T& a, const T& b) const {
            return a >= b;
        }
    };

    //! ----- Arithmetic ----- !//

    template <typename T>
    struct negate : public unary_function<T, T> {
        T operator()(const T& a) const {
            return -a;
        }
    };

    template <typename T>
    struct plus : public binary_function<T, T, T> {
        T operator()(const T& a, const T& b) const {
            return a + b;
        }
    };

    template <typename T>
    struct minus : public binary_function<T, T, T> {
        T operator()(const T& a, const T& b) const {
            return a - b;
        }
    };

    template <typename T>
    struct multiply : public binary_function<T, T, T> {
        T operator()(const T& a, const T& b) const {
            return a * b;
        }
    };

    template <typename T>
    struct divide : public binary_function<T, T, T> {
        T operator()(const T& a, const T& b) const {
            return a / b;
        }
    };

    template <typename T>
    struct modulu : public binary_function<T, T, T> {
        T operator()(const T& a, const T& b) const {
            return a % b;
        }
    };

    //! ----- Identity ----- !//

    template <typename T>
    inline T identity_element(plus<T>) {
        return T(0);
    };

    template <typename T>
    inline T identity_element(multiply<T>) {
        return T(1);
    };

    //! ----- Logical ----- !//

    template <typename T>
    struct logical_and : public binary_function<T, T, bool> {
        bool operator()(const T& a, const T& b) const {
            return a && b;
        }
    };

    template <typename T>
    struct logical_or : public binary_function<T, T, bool> {
        bool operator()(const T& a, const T& b) const {
            return a || b;
        }
    };

    template <typename T>
    struct logical_not : public unary_function<T, bool> {
        bool operator()(const T& a) const {
            return !a;
        }
    };

    template <typename T>
    struct identity : public unary_function<T, T> {
        const T& operator()(const T& a) const {
            return a;
        }
    };

    // Select the first element in a pair.
    template <typename Pair>
    struct select1st : public unary_function<Pair, typename Pair::first_type> {
        const typename Pair::first_type& operator()(const Pair& a) const {
            return a.first;
        }
    };

    // Select the second element in a pair
    template <typename Pair>
    struct select2rd : public unary_function<Pair, typename Pair::second_type> {
        const typename Pair::second_type& operator()(const Pair& a) const {
            return a.second;
        }
    };

    // Return the first element.
    template <typename Arg1, typename Arg2>
    struct project1st : public binary_function<Arg1, Arg2, Arg1> {
        Arg1& operator()(Arg1& a, Arg2&) {
            return a;
        }
    };

    // Return the second element.
    template <typename Arg1, typename Arg2>
    struct project2rd : public binary_function<Arg1, Arg2, Arg2> {
        Arg2& operator()(Arg1&, Arg2& b) {
            return b;
        }
    };

    //! ----- Function Adaptor ----- !//

    template <typename Predicate>
    class unary_negate : public unary_function<typename Predicate::argument_type, bool> {
    protected:
        Predicate pred;

    public:
        explicit unary_negate(const Predicate& pred)
            : pred(pred) {}
        bool operator()(const typename Predicate::argument_type& x) const {
            return !pred(x);
        }
    };

    // Negate the return value, unary function.
    template <typename Predicate>
    inline unary_negate<Predicate> not1(const Predicate& pred) {
        return unary_negate<Predicate>(pred);
    }

    template <typename Predicate>
    class binary_negate : public binary_function<typename Predicate::first_argument_type, typename Predicate::second_argument_type, bool> {
    protected:
        Predicate pred;

    public:
        explicit binary_negate(const Predicate& pred)
            : pred(pred) {}
        bool operator()(const typename Predicate::first_argument_type& x, const typename Predicate::second_argument_type& y) const {
            return !pred(x, y);
        }
    };

    // Negate the return value, binary function.
    template <typename Predicate>
    inline binary_negate<Predicate> not2(const Predicate& pred) {
        return binary_negate<Predicate>(pred);
    }

    //! ----- Binding Argument ----- !//

    template <typename Operator>
    class binder1st : public unary_function<typename Operator::second_argument_type, typename Operator::return_type> {
    protected:
        Operator op;
        typename Operator::first_argument_type x;

    public:
        // When constructing this functor, we have got the first argument.
        binder1st(const Operator& op, const typename Operator::first_argument_type& value)
            : op(op), x(value) {}
        // Then `operator()` just need to get the second argument.
        typename Operator::return_type operator()(const typename Operator::second_argument_type& y) const {
            return op(x, y);
        }
    };

    // A binary functor whose first argument is bound value.
    template <typename Operator, typename T>
    inline binder1st<Operator> bind1st(const Operator& op, const T& value) {
        using Arg1 = typename Operator::first_argument_type;
        return binder1st<Operator>(op, Arg1(value));
    }

    template <typename Operator>
    class binder2nd : public unary_function<typename Operator::first_argument_type, typename Operator::return_type> {
    protected:
        Operator op;
        typename Operator::second_argument_type y;

    public:
        binder2nd(const Operator& op, const typename Operator::second_argument_type& value)
            : op(op), y(value) {}
        typename Operator::return_type operator()(const typename Operator::first_argument_type& x) const {
            return op(x, y);
        }
    };

    // A binary functor whose second argument is bound value.
    template <typename Operator, typename T>
    inline binder2nd<Operator> bind2nd(const Operator& op, const T& value) {
        using Arg2 = typename Operator::second_argument_type;
        return binder2nd<Operator>(op, Arg2(value));
    }

    //! ----- Compose ----- !//

    template <typename Operator1, typename Operator2>
    class unary_compose : public unary_function<typename Operator2::argument_type, typename Operator1::return_type> {
    protected:
        Operator1 op1;
        Operator2 op2;

    public:
        unary_compose(const Operator1& op1, const Operator2& op2)
            : op1(op1), op2(op2) {}
        typename Operator1::return_type operator()(const typename Operator2::argument_type& x) const {
            return op1(op2(x));
        }
    };

    // op1(op2(x))
    template <typename Operator1, typename Operator2>
    inline unary_compose<Operator1, Operator2> compose1(const Operator1& op1, const Operator2& op2) {
        return unary_compose<Operator1, Operator2>(op1, op2);
    }

    template <typename Operator1, typename Operator2, typename Operator3>
    class binary_compose : public unary_function<typename Operator2::argument_type, typename Operator1::return_type> {
    protected:
        Operator1 op1;
        Operator2 op2;
        Operator3 op3;

    public:
        binary_compose(const Operator1& op1, const Operator2& op2, const Operator3& op3)
            : op1(op1), op2(op2), op3(op3) {}
        typename Operator1::return_type operator()(const typename Operator2::argument_type& x) const {
            return op1(op2(x), op3(x));
        }
    };

    // op1(op2(x), op3(x))
    template <typename Operator1, typename Operator2, typename Operator3>
    inline binary_compose<Operator1, Operator2, Operator3> compose2(const Operator1& op1, const Operator2& op2, const Operator3& op3) {
        return binary_compose<Operator1, Operator2, Operator3>(op1, op2, op3);
    }

    //! ----- Pointer to Function ----- !//

    // Turns a raw function pointer into a functor.
    template <typename Arg, typename Return>
    class pointer_to_unary_function : public unary_function<Arg, Return> {
    protected:
        // A function pointer whose argument type is `Arg` and return type is `Return`.
        Return (*ptr)(Arg);

    public:
        explicit pointer_to_unary_function(Return (*ptr)(Arg))
            : ptr(ptr) {}
        Return operator()(Arg x) const { return ptr(x); }
    };

    template <typename Arg, typename Return>
    inline pointer_to_unary_function<Arg, Return> ptr_fun(Return (*ptr)(Arg)) {
        return pointer_to_unary_function<Arg, Return>(ptr);
    }

    template <typename Arg1, typename Arg2, typename Return>
    class pointer_to_binary_function : public binary_function<Arg1, Arg2, Return> {
    protected:
        Return (*ptr)(Arg1, Arg2);

    public:
        explicit pointer_to_binary_function(Return (*ptr)(Arg1, Arg2))
            : ptr(ptr) {}
        Return operator()(Arg1 x, Arg2 y) const { return ptr(x, y); }
    };

    template <typename Arg1, typename Arg2, typename Return>
    inline pointer_to_binary_function<Arg1, Arg2, Return> ptr_fun(Return (*ptr)(Arg1, Arg2)) {
        return pointer_to_binary_function<Arg1, Arg2, Return>(ptr);
    }

    //! ----- Pointer to Member Function ----- !//

    // Turns member functions into a functor.
    // 8 = 2^3 functors
    // 1. no argument vs 1 argument
    // 2. call through pointer vs call through reference
    // 3. non-const member function vs const member function

    // No argument, call by pointer, non-const member function.
    template <typename Return, typename Type>
    class mem_fun_t : public unary_function<Type*, Return> {
    private:
        Return (Type::*member_fun)();

    public:
        // A function pointer which is a member of class `Type`, and whose return type is `Return`.
        explicit mem_fun_t(Return (Type::*ptr)())
            : member_fun(ptr) {}
        Return operator()(Type* instance) const { return (instance->*member_fun)(); }
    };

    // No argument, call by pointer, const member function.
    template <typename Return, typename Type>
    class const_mem_fun_t : public unary_function<const Type*, Return> {
    private:
        Return (Type::*member_fun)() const;

    public:
        explicit const_mem_fun_t(Return (Type::*ptr)() const)
            : member_fun(ptr) {}
        Return operator()(const Type* instance) const { return (instance->*member_fun)(); }
    };

    // No argument, call by reference, non-const member function.
    template <typename Return, typename Type>
    class mem_fun_ref_t : unary_function<Type, Return> {
    private:
        Return (Type::*member_fun)();

    public:
        explicit mem_fun_ref_t(Return (Type::*ptr)())
            : member_fun(ptr) {}
        Return operator()(Type& instance) const { return (instance.*member_fun)(); }
    };

    // No argument, call by reference, const member function.
    template <typename Return, typename Type>
    class const_mem_fun_ref_t : unary_function<Type, Return> {
    private:
        Return (Type::*member_fun)() const;

    public:
        explicit const_mem_fun_ref_t(Return (Type::*ptr)() const)
            : member_fun(ptr) {}
        Return operator()(const Type& instance) const { return (instance.*member_fun)(); }
    };

    // One argument, call by pointer, non-const member function.
    template <typename Return, typename Type, typename Arg>
    class mem_fun1_t : public binary_function<Type*, Arg, Return> {
    private:
        Return (Type::*member_fun)(Arg);

    public:
        explicit mem_fun1_t(Return (Type::*ptr)(Arg))
            : member_fun(ptr) {}
        Return operator()(Type* instance, Arg x) const { return (instance->*member_fun)(x); }
    };

    // One argument, call by pointer, const member function.
    template <typename Return, typename Type, typename Arg>
    class const_mem_fun1_t : public binary_function<Type*, Arg, Return> {
    private:
        Return (Type::*member_fun)(Arg) const;

    public:
        explicit const_mem_fun1_t(Return (Type::*ptr)(Arg) const)
            : member_fun(ptr) {}
        Return operator()(const Type* instance, Arg x) const { return (instance->*member_fun)(x); }
    };

    // One argument, call by reference, non-const member function.
    template <typename Return, typename Type, typename Arg>
    class mem_fun1_ref_t : binary_function<Type, Arg, Return> {
    private:
        Return (Type::*member_fun)(Arg);

    public:
        explicit mem_fun1_ref_t(Return (Type::*ptr)(Arg))
            : member_fun(ptr) {}
        Return operator()(Type& instance, Arg x) const { return (instance.*member_fun)(x); }
    };

    // One argument, call by reference, const member function.
    template <typename Return, typename Type, typename Arg>
    class const_mem_fun1_ref_t : binary_function<Type, Arg, Return> {
    private:
        Return (Type::*member_fun)(Arg) const;

    public:
        explicit const_mem_fun1_ref_t(Return (Type::*ptr)(Arg) const)
            : member_fun(ptr) {}
        Return operator()(const Type& instance, Arg x) const { return (instance.*member_fun)(x); }
    };

    template <typename Return, typename Type>
    inline mem_fun_t<Return, Type> mem_fun(Return (Type::*ptr)()) {
        return mem_fun_t<Return, Type>(ptr);
    }

    template <typename Return, typename Type>
    inline const_mem_fun_t<Return, Type> mem_fun(Return (Type::*ptr)() const) {
        return const_mem_fun_t<Return, Type>(ptr);
    }

    template <typename Return, typename Type>
    inline mem_fun_ref_t<Return, Type> mem_fun_ref(Return (Type::*ptr)()) {
        return mem_fun_ref_t<Return, Type>(ptr);
    }

    template <typename Return, typename Type>
    inline const_mem_fun_ref_t<Return, Type> mem_fun_ref(Return (Type::*ptr)() const) {
        return const_mem_fun_ref_t<Return, Type>(ptr);
    }

    template <typename Return, typename Type, typename Arg>
    inline mem_fun1_t<Return, Type, Arg> mem_fun(Return (Type::*ptr)(Arg)) {
        return mem_fun1_t<Return, Type, Arg>(ptr);
    }

    template <typename Return, typename Type, typename Arg>
    inline const_mem_fun1_t<Return, Type, Arg> mem_fun(Return (Type::*ptr)(Arg) const) {
        return const_mem_fun1_t<Return, Type, Arg>(ptr);
    }

    template <typename Return, typename Type, typename Arg>
    inline mem_fun1_ref_t<Return, Type, Arg> mem_fun_ref(Return (Type::*ptr)(Arg)) {
        return mem_fun1_ref_t<Return, Type, Arg>(ptr);
    }

    template <typename Return, typename Type, typename Arg>
    inline const_mem_fun1_ref_t<Return, Type, Arg> mem_fun_ref(Return (Type::*ptr)(Arg) const) {
        return const_mem_fun1_ref_t<Return, Type, Arg>(ptr);
    }

} // namespace TinySTL

#endif // !_TINYSTL_FUNCTION_HPP_
