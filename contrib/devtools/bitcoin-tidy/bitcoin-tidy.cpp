// Copyright (c) 2023 Mippscoin Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "logprintf.h"

#include <clang-tidy/ClangTidyModule.h>
#include <clang-tidy/ClangTidyModuleRegistry.h>

class MippscoinModule final : public clang::tidy::ClangTidyModule
{
public:
    void addCheckFactories(clang::tidy::ClangTidyCheckFactories& CheckFactories) override
    {
        CheckFactories.registerCheck<mippscoin::LogPrintfCheck>("mippscoin-unterminated-logprintf");
    }
};

static clang::tidy::ClangTidyModuleRegistry::Add<MippscoinModule>
    X("mippscoin-module", "Adds mippscoin checks.");

volatile int MippscoinModuleAnchorSource = 0;
