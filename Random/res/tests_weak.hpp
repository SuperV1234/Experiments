// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./shared.hpp"
#include "./legacy.hpp"
#include "./behavior.hpp"
#include "./unique_resource.hpp"
#include "./shared_resource.hpp"
#include "./weak.hpp"
#include "./make_resource.hpp"
#include "./access.hpp"
#include "./interface.hpp"
#include "./tests.hpp"

namespace test
{
    void weak_0()
    {
        assert_ck(0, 0);

        {
            auto h = test_behavior::init();
            assert_ck(1, 0);

            shared_test s0(h);
            assert_ck(1, 0);

            weak_test w0(s0);
            assert_ck(1, 0);

            assert(s0.use_count() == w0.use_count());
        }

        assert_ck(1, 1);
    }

    void weak_1()
    {
        assert_ck(0, 0);

        {
            shared_test s0(test_behavior::init());
            assert_ck(1, 0);

            weak_test w0(s0);
            assert_ck(1, 0);

            auto w1(w0);
            assert_ck(1, 0);

            assert(w0.lock().get() == w1.lock().get());
        }

        assert_ck(1, 1);
    }

    void weak_2()
    {
        assert_ck(0, 0);

        {
            shared_test s0(test_behavior::init());
            assert_ck(1, 0);

            weak_test w0(s0);
            assert_ck(1, 0);

            assert(w0.use_count() == 1);
        }

        assert_ck(1, 1);
    }

    void weak_3()
    {
        assert_ck(0, 0);

        {
            weak_test w0;

            {
                shared_test s0(test_behavior::init());
                assert_ck(1, 0);

                w0 = s0;
                assert_ck(1, 0);

                assert(!w0.expired());
            }

            assert(w0.expired());
        }

        assert_ck(1, 1);
    }

    void weak_4()
    {
        assert_ck(0, 0);

        {
            shared_test s0(test_behavior::init());
            assert_ck(1, 0);

            weak_test w0(s0);
            assert_ck(1, 0);

            assert(!w0.lock().unique());
        }

        assert_ck(1, 1);
    }

    void weak_5()
    {
        assert_ck(0, 0);

        {
            shared_test s0(test_behavior::init());
            assert_ck(1, 0);

            weak_test w0(s0);
            assert_ck(1, 0);

            shared_test s1(test_behavior::init());
            assert_ck(2, 0);
                 
            weak_test w1(s1);
            assert_ck(2, 0);
    
            auto old0(s0.get());
            auto old1(s1.get());
            assert_ck(2, 0);

            assert(w0.lock().get() == old0);
            assert(w1.lock().get() == old1);

            w0.swap(w1);
            assert_ck(2, 0);

            assert(w0.lock().get() == old1);
            assert(w1.lock().get() == old0);
        }

        assert_ck(2, 2);
    }
}