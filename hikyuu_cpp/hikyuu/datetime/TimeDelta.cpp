/*
 * TimeDelta.h
 *
 *  Copyright(C) 2019, hikyuu.org
 *
 *  Created on: 2019-12-12
 *      Author: fasiondog
 */

#include <cstdint>
#include "TimeDelta.h"
#include "../utilities/exception.h"

namespace hku {

TimeDelta::TimeDelta(int64_t days, int64_t hours, int64_t minutes, int64_t seconds,
                     int64_t milliseconds, int64_t microseconds) {
    HKU_CHECK(days <= 99999999 && days >= -99999999, "Out of range! Input days: {}", days);
    HKU_CHECK(hours >= -100000 && hours <= 100000, "Out of range! Input hours: {}", hours);
    HKU_CHECK(minutes >= -100000 && minutes <= 100000, "Out of range! Input minutes: {}", minutes);
    HKU_CHECK(seconds >= -8639900 && seconds <= 8639900, "Out of range! Input seconds: {}",
              seconds);
    HKU_CHECK(milliseconds >= -86399000000 && milliseconds <= 86399000000,
              "Out of range! Input milliseconds: {}", milliseconds);
    HKU_CHECK(microseconds >= -86399000000 && microseconds <= 86399000000,
              "Out of range! Input microseconds: {}", microseconds);
    int64_t total =
      ((((days * 24 + hours) * 60 + minutes) * 60 + seconds) * 1000 + milliseconds) * 1000 +
      microseconds;
    HKU_CHECK(total >= m_min_micro_seconds && total <= m_max_micro_seconds, "Out of total range!");
    m_duration = bt::time_duration(0, 0, 0, total);
}

TimeDelta::TimeDelta(bt::time_duration td) {
    int64_t total = td.total_microseconds();
    HKU_CHECK(total >= m_min_micro_seconds && total <= m_max_micro_seconds, "Out of total range!");
    m_duration = td;
}

int64_t TimeDelta::days() const {
    if (isNegative()) {
        if (ticks() % m_one_day_ticks == 0) {
            return ticks() / m_one_day_ticks;
        } else {
            return ticks() / m_one_day_ticks - 1;
        }
    }
    return std::abs(m_duration.hours() / 24);
}

int64_t TimeDelta::hours() const {
    if (isNegative()) {
        if (ticks() % m_one_day_ticks == 0) {
            return 0;
        } else {
            int64_t pos_ticks =
              std::abs((ticks() / m_one_day_ticks - 1) * m_one_day_ticks) + ticks();
            return bt::time_duration(0, 0, 0, pos_ticks).hours();
        }
    }
    return std::abs(m_duration.hours()) % 24;
}

int64_t TimeDelta::minutes() const {
    if (isNegative()) {
        if (ticks() % m_one_day_ticks == 0) {
            return 0;
        } else {
            int64_t pos_ticks =
              std::abs((ticks() / m_one_day_ticks - 1) * m_one_day_ticks) + ticks();
            return bt::time_duration(0, 0, 0, pos_ticks).minutes();
        }
    }
    return std::abs(m_duration.minutes());
}

int64_t TimeDelta::seconds() const {
    if (isNegative()) {
        if (ticks() % m_one_day_ticks == 0) {
            return 0;
        } else {
            int64_t pos_ticks =
              std::abs((ticks() / m_one_day_ticks - 1) * m_one_day_ticks) + ticks();
            return bt::time_duration(0, 0, 0, pos_ticks).seconds();
        }
    }
    return std::abs(m_duration.seconds());
}

int64_t TimeDelta::milliseconds() const {
    if (isNegative()) {
        if (ticks() % m_one_day_ticks == 0) {
            return 0;
        } else {
            int64_t pos_ticks =
              std::abs((ticks() / m_one_day_ticks - 1) * m_one_day_ticks) + ticks();
            int64_t milli = pos_ticks % 1000000;
            return milli == 0 ? 0 : (milli - microseconds()) / 1000;
        }
    }
    return (std::abs(ticks()) % 1000000 - microseconds()) / 1000;
}

int64_t TimeDelta::microseconds() const {
    if (isNegative()) {
        if (ticks() % m_one_day_ticks == 0) {
            return 0;
        } else {
            int64_t micro = ticks() % 1000;
            return micro == 0 ? micro : 1000 + micro;
        }
    }
    return std::abs(ticks() % 1000);
}

} /* namespace hku */