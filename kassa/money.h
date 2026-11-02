#pragma once
#include <QRegularExpression>
#include <QString>
#include <algorithm>
#include <array>
#include <vector>

namespace Money
{
using Amount = qint64;
// Individual entries are bounded; all arithmetic is in kopecks.
inline bool parse(QString text, Amount &result, bool allowNegative = false)
{
    text = text.trimmed().replace(',', '.');
    static const QRegularExpression format(QStringLiteral("^-?[0-9]{1,9}(?:[.][0-9]{1,2})?$"));
    if (!format.match(text).hasMatch() || (!allowNegative && text.startsWith('-')))
        return false;
    const bool negative = text.startsWith('-');
    if (negative)
        text.remove(0, 1);
    const auto parts = text.split('.');
    result = parts[0].toLongLong() * 100;
    if (parts.size() == 2)
        result += parts[1].leftJustified(2, '0').toLongLong();
    if (negative)
        result = -result;
    return true;
}
inline QString format(Amount value)
{
    const Amount absolute = value < 0 ? -value : value;
    return (value < 0 ? QStringLiteral("-") : QString()) + QString::number(absolute / 100) + "." +
           QString::number(absolute % 100).rightJustified(2, '0');
}
struct Transfer
{
    int from;
    int to;
    Amount amount;
};
inline std::array<Amount, 3> shares(Amount total)
{
    std::array<Amount, 3> result{total / 3, total / 3, total / 3};
    for (int i = 0; i < total % 3; ++i)
        ++result[i];
    return result;
}
// Positive balance means money held by a participant, to be transferred out.
inline std::vector<Transfer> settle(std::array<Amount, 4> balance)
{
    balance[0] = -(balance[1] + balance[2] + balance[3]);
    std::vector<Transfer> result;
    for (int from = 0; from < 4; ++from)
        for (int to = 0; to < 4; ++to)
        {
            if (balance[from] <= 0 || balance[to] >= 0)
                continue;
            const Amount amount = std::min(balance[from], -balance[to]);
            result.push_back({from, to, amount});
            balance[from] -= amount;
            balance[to] += amount;
        }
    return result;
}
} // namespace Money
