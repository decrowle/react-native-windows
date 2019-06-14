// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "ThemingModule.h"

namespace facebook {
  namespace react {

    //
    // Theming and High Contrast
    //

    Theming::Theming() = default;
    Theming::~Theming() = default;

    // TODO: real implementation
    const char* Theming::getTheme()
    {
      return "light";
    }

    const char* Theming::getHighContrast()
    {
      return "false";
    }

    //
    // ThemingModule
    //
    const char* ThemingModule::name = "Theming";

    AppStateModule::AppStateModule(std::shared_ptr<AppState>&& appState)
      : m_appState(std::move(appState))
    {
    }

    std::string ThemingModule::getName()
    {
      return name;
    }

    std::map<std::string, folly::dynamic> ThemingModule::getConstants()
    {
      return { {"initialAppState", m_appState->getState()} };
    }

    auto ThemingModule::getMethods() -> std::vector<facebook::xplat::module::CxxModule::Method>
    {
      return {
        Method("getCurrentTheme", [this](folly::dynamic args, Callback cbSuccess, Callback /*cbFailure*/)
        {
          cbSuccess({folly::dynamic::object("theme", m_theme->getTheme())});
        }, AsyncTag),
        Method("getHighContrast", [this](folly::dynamic args, Callback cbSuccess, Callback /*cbFailure*/)
        {
          cbSuccess({folly::dynamic::object("highContrast", m_highContast->getHighContrast())});
        }, AsyncTag),
      };
    }

  }
} // namespace facebook::react
