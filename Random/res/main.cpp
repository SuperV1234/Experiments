// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#include "./shared.hpp"
#include "./legacy.hpp"
#include "./behavior.hpp"
#include "./unique_resource.hpp"
#include "./make_resource.hpp"
#include "./access.hpp"

template <                                // .
    template <typename> class TInterface, // .
    template <typename> class TAccess,    // .
    typename TBehavior                    // .
    >
class interface_maker
{
public:
    using behavior_type = TBehavior;
    using access_type = TAccess<TBehavior>;
    using interface_type = TInterface<access_type>;

private:
    template <typename... Ts>
    decltype(auto) init_resource(Ts&&... xs) noexcept(noexcept(true))
    {
        return behavior_type::init(FWD(xs)...);
    }

public:
    template <typename... Ts>
    auto operator()(Ts&&... xs) noexcept(noexcept(true))
    {
        return interface_type{init_resource(FWD(xs)...)};
    }
};

template <                                // .
    typename TBehavior,                   // .
    template <typename> class TInterface, // .
    template <typename> class TAccess,    // .
    typename... Ts                        // .
    >
auto make_interface(Ts&&... xs) noexcept(noexcept(true))
{
    return interface_maker<TInterface, TAccess, TBehavior>{}(FWD(xs)...);
}

template <typename TAccess>
struct vbo_interface : TAccess
{
    using TAccess::TAccess;

    void my_interface_method_0()
    {
        // something(_access.handle());
    }
};

int main()
{


    {
        auto x(
            make_interface<behavior::vbo_b, vbo_interface, access::unmanaged>(
                2));
        x.my_interface_method_0();

        behavior::vbo_b{}.deinit(x.handle());
    }

    {
        auto x(
            make_interface<behavior::vbo_b, vbo_interface, access::unique>(2));
        x.my_interface_method_0();
    }

    auto test0 = make_unique_resource<behavior::vbo_b>(1);



    return 0;
}

// TODO:
// ??