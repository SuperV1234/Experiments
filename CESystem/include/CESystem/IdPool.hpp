// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef CESYSTEM_IDPOOL
#define CESYSTEM_IDPOOL

namespace ssvces
{
    namespace Impl
    {
        class IdPool
        {
            // IdPool stores available Entity ids and is used to check Entity
            // validity

        private:
            std::vector<EntityId> available;
            std::array<EntityIdCtr, maxEntities> counters;

        public:
            inline IdPool() : available(maxEntities)
            {
                // Fill the available ids vector from 0 to maxEntities
                ssvu::iota(available, 0);
                ssvu::fill(counters, 0);
            }

            // Returns the first available IdCtrPair
            inline EntityStat getAvailable() noexcept
            {
                SSVU_ASSERT(!available.empty());

                EntityId id(available.back());
                available.pop_back();
                return {id, counters[id]};
            }

            // Used on Entity death, reclaims the Entity's id so that it can be
            // reused
            inline void reclaim(const EntityStat& mStat) noexcept
            {
                if(mStat.ctr != counters[mStat.id]) return;
                ++counters[mStat.id];
                available.emplace_back(mStat.id);
            }

            // Checks if an Entity is currently alive
            inline bool isAlive(const EntityStat& mStat) const noexcept
            {
                return counters[mStat.id] == mStat.ctr;
            }
        };
    }
}

#endif
