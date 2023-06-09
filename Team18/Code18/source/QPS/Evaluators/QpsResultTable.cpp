#include "QpsResultTable.h"

#include <stdexcept>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <cassert>

#include "HashTuple.h"

spa::QpsResultRow spa::QpsFilteredRow::getFilteredRow() const {
  QpsResultRow result;
  for (int i : selectedColumns) {
    result.push_back(row[i]);
  }
  return result;
}

std::pair<std::vector<int>, std::vector<int>> spa::QpsResultTable::getCommonColumnIndices(QpsResultTable& table,
                                                                                          QpsResultTable& other) {
  std::vector<int> tIndices;
  std::vector<int> oIndices;
  auto& tMap = table.headerIndexMap;
  auto& oMap = other.headerIndexMap;
  for (auto it = tMap.begin(); it != tMap.end(); ++it) {
    auto otherIt = oMap.find(it->first);
    if (otherIt == oMap.end()) {
      continue;
    }
    tIndices.push_back((it->second)[0]);
    oIndices.push_back((otherIt->second)[0]);
  }
  return { tIndices, oIndices };
}

spa::QpsResultRow spa::QpsResultTable::concatenateRow(QpsResultRow& row, QpsResultRow& other) {
  spa::QpsResultRow result;
  for (auto& v : row) {
    result.push_back(v);
  }
  for (auto& v : other) {
    result.push_back(v);
  }
  return result;
}

std::vector<std::string> spa::QpsResultTable::getHeaderNames() {
  std::vector<std::string> result;
  for (auto& p : headerIndexMap) {
    result.push_back(p.first);
  }
  return result;
}

void spa::QpsResultTable::addEmptyHeader() {
  headers.push_back("");
}

void spa::QpsResultTable::addHeader(const std::string& header) {
  if (!header.empty()) {
    headerIndexMap[header].push_back(headers.size());
  }
  headers.push_back(header);
}

void spa::QpsResultTable::addHeader(PqlArgument arg) {
  if (arg.getType() != SYNONYM) {
    return headers.push_back("");
  }
  addHeader(arg.getValue());
}

bool spa::QpsResultTable::hasHeader(const std::string& header) {
  return headerIndexMap.find(header) != headerIndexMap.end();
}

std::pair<int, int> spa::QpsResultTable::getDimension() {
  return { headers.size(), rows.size() };
}

void spa::QpsResultTable::addDummyRow() {
  rows.push_back(std::vector<QpsValue>(headers.size(), QpsValue(0)));
}

void spa::QpsResultTable::addRow(const QpsResultRow& row) {
  assert(row.size() == headers.size());
  rows.push_back(row);
}

spa::QpsValueSet spa::QpsResultTable::getColumn(std::string header) {
  QpsValueSet result;
  auto it = headerIndexMap.find(header);
  if (it == headerIndexMap.end()) {
    return {};
  }
  int index = it->second[0];
  for (auto& row : rows) {
    result.insert(row[index]);
  }
  return result;
}

spa::QpsResultTable spa::QpsResultTable::getColumns(const std::vector<std::string>& headers) {
  QpsResultTable result;
  std::vector<int> indices;
  for (auto& header : headers) {
    auto it = headerIndexMap.find(header);
    if (it == headerIndexMap.end()) {
      continue;
    }
    result.addHeader(header);
    indices.push_back(it->second[0]);
  }
  if (indices.empty()) {
    return result;
  }
  QpsRowSet rowSet;
  for (auto& row : rows) {
    rowSet.insert({ indices, row });
  }
  for (auto& fRow : rowSet) {
    result.addRow(fRow.getFilteredRow());
  }
  return result;
}

void spa::QpsResultTable::cartesianProduct(QpsResultTable& result, QpsResultTable& other) {
  for (auto& row : rows) {
    for (auto& oRow : other.rows) {
      result.addRow(concatenateRow(row, oRow));
    }
  }
}

size_t spa::QpsFilteredRowHash::operator()(const QpsFilteredRow& r) const {
  size_t result = 0;
  QpsValueHash valHash;
  for (int i : r.selectedColumns) {
    spa::hash_combine<size_t>(result, valHash(r.row[i]));
  }
  return result;
}

bool spa::QpsFilteredRowEquality::operator()(const QpsFilteredRow& first, const QpsFilteredRow& second) const {
  auto& firstColumns = first.selectedColumns;
  auto& secondColumns = second.selectedColumns;
  if (firstColumns.size() != secondColumns.size()) {
    return false;
  }
  for (size_t i = 0; i < firstColumns.size(); ++i) {
    if (first.row[firstColumns[i]] != second.row[secondColumns[i]]) {
      return false;
    }
  }
  return true;
}

void spa::QpsResultTable::fillQpsRowMap(QpsRowMap& m, QpsResultTable& table, std::vector<int>& selectedColumns) {
  for (auto& row : table.rows) {
    m[{selectedColumns, row}].push_back(&row);
  }
}

spa::QpsResultTable spa::QpsResultTable::innerJoin(QpsResultTable& other) {
  QpsResultTable resultTable;
  for (auto& s : headers) {
    resultTable.addHeader(s);
  }
  for (auto& s : other.headers) {
    resultTable.addHeader(s);
  }
  auto commonIndices = getCommonColumnIndices(*this, other);
  if (commonIndices.first.empty()) {
    cartesianProduct(resultTable, other);
    return resultTable;
  }
  QpsRowMap currMap;
  fillQpsRowMap(currMap, *this, commonIndices.first);
  QpsRowMap otherMap;
  fillQpsRowMap(otherMap, other, commonIndices.second);
  for (auto& currP : currMap) {
    auto it = otherMap.find(currP.first);
    if (it == otherMap.end()) {
      continue;
    }
    for (auto rowPtr : currP.second) {
      for (auto otherRowPtr : it->second) {
        resultTable.addRow(concatenateRow(*rowPtr, *otherRowPtr));
      }
    }
  }
  return resultTable;
}

const std::vector<spa::QpsResultRow>& spa::QpsResultTable::getRows() {
  return rows;
}

bool spa::QpsResultTable::isEmpty() {
  return rows.size() == 0;
}

spa::QpsResultTable spa::QpsResultTable::matchColumns(const std::string& first, const std::string& second) {
  int firstIndex = headerIndexMap[first][0];
  int secondIndex = headerIndexMap[second][0];
  QpsResultTable result;
  for (auto& header : headers) {
    result.addHeader(header);
  }
  for (auto& row : rows) {
    if (row[firstIndex] == row[secondIndex]) {
      result.addRow(row);
    }
  }
  return result;
}
