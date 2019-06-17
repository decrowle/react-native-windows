// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "pch.h"
#include "ThemingModuleUwp.h"

#include <winrt/Windows.UI.Xaml.h>

namespace react {
  namespace uwp {

    //
    // Theming
    //

    Theming::Theming(const std::shared_ptr<IReactInstance>& reactInstance)
      : facebook::react::Theming()
      , m_wkReactInstance(reactInstance)
    {
      m_lastTheme = "light";
      m_lastHighContrastState = "false";

      m_enteredLightModeRevoker = winrt::Windows::UI::Xaml::Application::Current().EnteredBackground(winrt::auto_revoke, { this, &Theming::EnteredLightMode });
      m_enteredDarkModeRevoker = winrt::Windows::UI::Xaml::Application::Current().LeavingBackground(winrt::auto_revoke, { this, &Theming::EnteredDarkMode });
      m_highContrastChangedRevoker = winrt::Windows::UI::Xaml::Application::Current().EnteredBackground(winrt::auto_revoke, { this, &Theming::HighContrastActivated });
    }
    }

    Theming::~Theming() = default;

    const char* Theming::getTheme()
    {
      return m_lastTheme;
    }

    const char* Theming::getHighContrast()
    {
      return m_lastHighContrastState;
    }

    void Theming::EnteredLightMode(winrt::Windows::Foundation::IInspectable const& /*sender*/, winrt::Windows::ApplicationModel::VisualStateChangedEventArgs const& /*e*/)
    {
      fireThemeEvent("light");
    }

    void Theming::EnteredDarkMode(winrt::Windows::Foundation::IInspectable const& /*sender*/, winrt::Windows::ApplicationModel::VisualStateChangedEventArgs const& /*e*/)
    {
      fireThemeEvent("dark");
    }

    void Theming::HighContrastChanged(winrt::Windows::Foundation::IInspectable const& /*sender*/, winrt::Windows::ApplicationModel::VisualStateChangedEventArgs const& /*e*/)
    {
      if (m_lastHighContrastState == "true") {

        fireHighContrastEvent("false");
      }
      else
      {
        fireHighContrastEvent("true");
      }
    }

    void Theming::fireThemeEvent(const char* newTheme)
    {
      auto instance = m_wkReactInstance.lock();
      if (instance)
      {
        m_lastTheme = newTheme;
        folly::dynamic parameters = folly::dynamic::object("theme", newTheme);
        instance->CallJsFunction("RCTDeviceEventEmitter", "emit", folly::dynamic::array("themeDidChange", std::move(parameters)));
      }
    }

    void Theming::fireHighContrastEvent(const char* newHighContrastState)
    {
      auto instance = m_wkReactInstance.lock();
      if (instance)
      {
        m_lastHighContrastState = newHighContrastState;
        folly::dynamic parameters = folly::dynamic::object("highContrast", newHighContrastState);
        instance->CallJsFunction("RCTDeviceEventEmitter", "emit", folly::dynamic::array("highContrastDidChange", std::move(parameters)));
      }
    }

  }
} // namespace react::uwp
