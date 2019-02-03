//
// Created by howorang on 26.01.19.
//

#include "SymbolTable.h"

SymbolTable symbolTable = SymbolTable();

int SymbolTable::insert(const std::string symbol, int tokenType) {
    SymbolEntry entry = {.tokenType = tokenType, .tokenVal = symbol, .isLocal = !global};
    table.push_back(entry);
    return static_cast<int>(table.size() - 1);
}

int SymbolTable::lookup(const std::string symbol) {
    for (int i = table.size() - 1; i >= 0; i--) {
        if (table[i].isLocal == !global && table[i].tokenVal == symbol) {
            return i;
        }
    }
    return -1;
}

SymbolTable::SymbolEntry &SymbolTable::operator[](int i) {
    return table[i];
}

void SymbolTable::initDeclarationList(std::vector<int> symbolIndexes, int type) {
    for (int symbolIndex : symbolIndexes) {
        SymbolEntry &entry = operator[](symbolIndex);
        entry.varType = type;
        entry.place = getPlace(type);
    }
}

int SymbolTable::insertLiteral(std::string value) {
    SymbolEntry entry = {.tokenVal = std::move(value),
            .isLiteral = true};
    table.push_back(entry);
    return static_cast<int>(table.size() - 1);
}

int SymbolTable::insertTempVar(int type) {
    SymbolEntry entry = {.varType = type, .place = getPlace(type)};
    table.push_back(entry);
    return static_cast<int>(table.size() - 1);
}


int SymbolTable::getPlace(int type) {
    int toReturn = lastFreeMemAddr;
    lastFreeMemAddr += sizeOfSymbol(type);
    return toReturn;
}

void SymbolTable::toggleGlobal() {
    if (global) {
        oldLastFreeMemAddr = lastFreeMemAddr;
        lastFreeMemAddr = 0;
    } else {
        lastFreeMemAddr = oldLastFreeMemAddr;
        removeLocalSymbols();
    }
    global = !global;
}

void SymbolTable::removeLocalSymbols() {
    table.erase(
            std::remove_if(
                    table.begin(), table.end(),
                    [](SymbolEntry &entry) { return entry.isLocal; }),
            table.end());
}


bool SymbolTable::isGlobal() {
    return global;
}

void SymbolTable::initSubProgram(int index, std::vector<std::pair<int, std::vector<int>>> paramListHolder) {
    int incsp = SUBPROGRAM_OFFSET;
    SymbolEntry &symbolEntry = operator[](index);
    if (symbolEntry.isProcedure) {
    } else {
        //PUTTING IN TEMP VAR FOR FUNCTION RESULT
        incsp += 4;
        SymbolEntry entry = {
                .tokenVal = symbolEntry.tokenVal,
                .varType = symbolEntry.varType,
                .place = incsp,
                .isLocal = true,
                .isRef = true};
        table.push_back(entry);
    }

    for (const auto &type_indexes : paramListHolder) {
        for (const auto &symbolIndex : type_indexes.second) {
            incsp += 4;
            SymbolEntry &entry = operator[](symbolIndex);
            entry.isRef = true;
            entry.isLocal = true;
            entry.place = incsp;
            entry.varType = type_indexes.first;
        }
    }
    symbolEntry.incsp = incsp - 4;
}

int SymbolTable::getPlace(SymbolTable::SymbolEntry entry) {
    if (entry.isRef) {
        return 4;
    } else {
        return getPlace(entry.varType);
    }
}

int SymbolTable::getAllocatedMem() {
    return lastFreeMemAddr;
}
