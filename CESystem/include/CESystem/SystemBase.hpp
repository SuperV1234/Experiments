// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef CESYSTEM_SYSTEMBASE
#define CESYSTEM_SYSTEMBASE

namespace ssvces
{
    namespace Impl
    {
        class SystemBase
        {
            friend bool matchesSystem(
                const TypeIdxBitset&, const SystemBase&) noexcept;
            friend ssvces::Manager;

        private:
            TypeIdxBitset typeIdsReq, typeIdsNot;

        protected:
            inline SystemBase(const TypeIdxBitset& mTypeIdsReq)
                : typeIdsReq{mTypeIdsReq}
            {
            }
            inline SystemBase(const TypeIdxBitset& mTypeIdsReq,
                const TypeIdxBitset& mTypeIdsNot)
                : typeIdsReq{mTypeIdsReq}, typeIdsNot{mTypeIdsNot}
            {
            }
            inline virtual ~SystemBase() noexcept {}

            virtual void registerEntity(Entity&) = 0;
            virtual void refresh() = 0;

        public:
            inline SystemBase(const SystemBase&) = delete;
            inline SystemBase& operator=(const SystemBase&) = delete;
        };
    }
}

#endif
