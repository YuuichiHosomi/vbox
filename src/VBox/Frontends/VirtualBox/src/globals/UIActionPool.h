/** @file
 * VBox Qt GUI - UIActionPool class declaration.
 */

/*
 * Copyright (C) 2010-2014 Oracle Corporation
 *
 * This file is part of VirtualBox Open Source Edition (OSE), as
 * available from http://www.virtualbox.org. This file is free software;
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software
 * Foundation, in version 2 as it comes in the "COPYING" file of the
 * VirtualBox OSE distribution. VirtualBox OSE is distributed in the
 * hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
 */

#ifndef ___UIActionPool_h___
#define ___UIActionPool_h___

/* Qt includes: */
#include <QAction>
#include <QMenu>

/* GUI includes: */
#include "QIWithRetranslateUI.h"
#include "UIExtraDataDefs.h"

/* Forward declarations: */
class UIActionPolymorphic;
class UIActionPool;
class UIActionPoolRuntime;
class UIActionPoolSelector;


/** Action-pool types. */
enum UIActionPoolType
{
    UIActionPoolType_Selector,
    UIActionPoolType_Runtime
};

/** Action types. */
enum UIActionType
{
    UIActionType_Menu,
    UIActionType_Simple,
    UIActionType_Toggle,
    UIActionType_Polymorphic
};

/** Action indexes. */
enum UIActionIndex
{
#ifdef RT_OS_DARWIN
    /* 'Application' menu actions: */
    UIActionIndex_M_Application,
    UIActionIndex_M_Application_S_About,
    UIActionIndex_M_Application_S_Preferences,
    UIActionIndex_M_Application_S_Close,
#endif /* RT_OS_DARWIN */

    /* 'Help' menu actions: */
    UIActionIndex_Menu_Help,
    UIActionIndex_Simple_Contents,
    UIActionIndex_Simple_WebSite,
    UIActionIndex_Simple_ResetWarnings,
#ifdef VBOX_GUI_WITH_NETWORK_MANAGER
    UIActionIndex_Simple_NetworkAccessManager,
    UIActionIndex_Simple_CheckForUpdates,
#endif /* VBOX_GUI_WITH_NETWORK_MANAGER */
#ifndef RT_OS_DARWIN
    UIActionIndex_Simple_About,
    UIActionIndex_Simple_Preferences,
#endif /* !RT_OS_DARWIN */

    /* Maximum index: */
    UIActionIndex_Max
};

/** Restriction levels. */
enum UIActionRestrictionLevel
{
    UIActionRestrictionLevel_Base,
    UIActionRestrictionLevel_Session,
    UIActionRestrictionLevel_Logic
};


/** QMenu extension. */
class UIMenu : public QMenu
{
    Q_OBJECT;

public:

    /** Constructor. */
    UIMenu();

    /** Defines whether tool-tip should be shown. */
    void setShowToolTip(bool fShowToolTips) { m_fShowToolTip = fShowToolTips; }

#ifdef Q_WS_MAC
    /** Mac OS X: Returns whether this menu is consumable by the menu-bar. */
    bool isConsumable() const { return m_fConsumable; }
    /** Mac OS X: Defines whether this menu is @a fConsumable by the menu-bar. */
    void setConsumable(bool fConsumable) { m_fConsumable = fConsumable; }

    /** Mac OS X: Returns whether this menu is consumed by the menu-bar. */
    bool isConsumed() const { return m_fConsumed; }
    /** Mac OS X: Defines whether this menu is @a fConsumed by the menu-bar. */
    void setConsumed(bool fConsumed) { m_fConsumed = fConsumed; }
#endif /* Q_WS_MAC */

protected:

    /** General event handler. */
    virtual bool event(QEvent *pEvent);

private:

    /** Holds whether tool-tip should be shown. */
    bool m_fShowToolTip;

#ifdef Q_WS_MAC
    /** Mac OS X: Holds whether this menu can be consumed by the menu-bar. */
    bool m_fConsumable;
    /** Mac OS X: Holds whether this menu is consumed by the menu-bar. */
    bool m_fConsumed;
#endif /* Q_WS_MAC */
};


/** Abstract QAction extension. */
class UIAction : public QAction
{
    Q_OBJECT;

public:

    /** Returns action type. */
    UIActionType type() const { return m_type; }

    /** Returns menu contained by this action. */
    UIMenu* menu() const;

    /** Returns action-pool this action belongs to. */
    UIActionPool* actionPool() const { return m_pActionPool; }

    /** Casts action to polymorphic-action. */
    UIActionPolymorphic* toActionPolymorphic();

    /** Returns current action name. */
    const QString& name() const { return m_strName; }
    /** Defines current action name. */
    void setName(const QString &strName);

    /** Returns action extra-data ID. */
    virtual int extraDataID() const { return 0; }
    /** Returns action extra-data key. */
    virtual QString extraDataKey() const { return QString(); }
    /** Returns whether action is allowed. */
    virtual bool isAllowed() const { return true; }

    /** Returns extra-data ID to save keyboard shortcut under. */
    virtual QString shortcutExtraDataID() const { return QString(); }
    /** Returns default keyboard shortcut for this action. */
    virtual QKeySequence defaultShortcut(UIActionPoolType) const { return QKeySequence(); }

    /** Defines current keyboard shortcut for this action. */
    void setShortcut(const QKeySequence &shortcut);
    /** Make action show keyboard shortcut. */
    void showShortcut();
    /** Make action hide keyboard shortcut. */
    void hideShortcut();

    /** Retranslates action. */
    virtual void retranslateUi() = 0;

protected:

    /** Constructor. */
    UIAction(UIActionPool *pParent, UIActionType type);

    /** Returns current action name in menu. */
    QString nameInMenu() const;

    /** Updates action text accordingly. */
    virtual void updateText();

private:

    /** Holds the action type. */
    UIActionType m_type;

    /** Holds the reference to the action-pool this action belongs to. */
    UIActionPool *m_pActionPool;
    /** Holds the type of the action-pool this action belongs to. */
    UIActionPoolType m_actionPoolType;

    /** Holds the action name. */
    QString m_strName;
    /** Holds the action shortcut. */
    QKeySequence m_shortcut;
    /** Holds whether action shortcut hidden. */
    bool m_fShortcutHidden;
};


/** Abstract UIAction extension for 'Menu' action type. */
class UIActionMenu : public UIAction
{
    Q_OBJECT;

protected:

    /** Constructor, taking normal icon name and name for disabled analog. */
    UIActionMenu(UIActionPool *pParent,
                 const QString &strIcon = QString(), const QString &strIconDis = QString());
    /** Constructor, taking copy of existing icon. */
    UIActionMenu(UIActionPool *pParent,
                 const QIcon &icon);

    /** Defines whether tool-tip should be shown. */
    void setShowToolTip(bool fShowToolTip);

private:

    /** Prepare routine. */
    void prepare();

    /** Updates action text accordingly. */
    virtual void updateText();
};


/** Abstract UIAction extension for 'Simple' action type. */
class UIActionSimple : public UIAction
{
    Q_OBJECT;

protected:

    /** Constructor, taking normal icon name and name for disabled analog. */
    UIActionSimple(UIActionPool *pParent,
                   const QString &strIcon = QString(), const QString &strIconDisabled = QString());
    /** Constructor, taking normal, small icon names and names for disabled analogs. */
    UIActionSimple(UIActionPool *pParent,
                   const QString &strIconNormal, const QString &strIconSmall,
                   const QString &strIconNormalDisabled, const QString &strIconSmallDisabled);
    /** Constructor, taking copy of existing icon. */
    UIActionSimple(UIActionPool *pParent,
                   const QIcon& icon);
};


/** Abstract UIAction extension for 'Toggle' action type. */
class UIActionToggle : public UIAction
{
    Q_OBJECT;

protected:

    /** Constructor, taking normal icon name and name for disabled analog. */
    UIActionToggle(UIActionPool *pParent,
                   const QString &strIcon = QString(), const QString &strIconDisabled = QString());
    /** Constructor, taking normal on/off icon names and names for disabled analogs. */
    UIActionToggle(UIActionPool *pParent,
                   const QString &strIconOn, const QString &strIconOff,
                   const QString &strIconOnDisabled, const QString &strIconOffDisabled);
    /** Constructor, taking copy of existing icon. */
    UIActionToggle(UIActionPool *pParent,
                   const QIcon &icon);

private:

    /** Prepare routine. */
    void prepare();
};


/** Abstract UIAction extension for 'Polymorphic' action type. */
class UIActionPolymorphic : public UIAction
{
    Q_OBJECT;

public:

    /** Returns current action state. */
    int state() const { return m_iState; }
    /** Defines current action state. */
    void setState(int iState) { m_iState = iState; retranslateUi(); }

protected:

    /** Constructor, taking normal icon name and name for disabled analog. */
    UIActionPolymorphic(UIActionPool *pParent,
                  const QString &strIcon = QString(), const QString &strIconDisabled = QString());
    /** Constructor, taking normal, small icon names and names for disabled analogs. */
    UIActionPolymorphic(UIActionPool *pParent,
                  const QString &strIconNormal, const QString &strIconSmall,
                  const QString &strIconNormalDisabled, const QString &strIconSmallDisabled);
    /** Constructor, taking copy of existing icon. */
    UIActionPolymorphic(UIActionPool *pParent,
                  const QIcon& icon);

private:

    /** Holds current action state. */
    int m_iState;
};


/** Abstract QObject extension
  * representing action-pool interface and factory. */
class UIActionPool : public QIWithRetranslateUI3<QObject>
{
    Q_OBJECT;

    /** Pointer to menu update-handler for this class. */
    typedef void (UIActionPool::*PTFActionPool)();
    /** Pointer to menu update-handler for Runtime sub-class. */
    typedef void (UIActionPoolRuntime::*PTFActionPoolRuntime)();
    /** Union for two defines above. */
    union PointerToFunction
    {
        PTFActionPool ptf;
        PTFActionPoolRuntime ptfr;
    };

signals:

    /** Notifies about menu prepare. */
    void sigNotifyAboutMenuPrepare(int iIndex, QMenu *pMenu);

public:

    /** Static factory constructor. */
    static UIActionPool* create(UIActionPoolType type);
    /** Static factory destructor. */
    static void destroy(UIActionPool *pActionPool);

    /** Static factory constructor (temporary),
      * used to initialize shortcuts-pool from action-pool of passed @a type. */
    static void createTemporary(UIActionPoolType type);

    /** Cast action-pool to Runtime one. */
    UIActionPoolRuntime* toRuntime();
    /** Cast action-pool to Selector one. */
    UIActionPoolSelector* toSelector();

    /** Returns action-pool type. */
    UIActionPoolType type() const { return m_type; }

    /** Returns the action for the passed @a iIndex. */
    UIAction* action(int iIndex) const { return m_pool.value(iIndex); }
    /** Returns all the actions action-pool contains. */
    QList<UIAction*> actions() const { return m_pool.values(); }

    /** Returns whether the menu with passed @a type is allowed in menu-bar. */
    bool isAllowedInMenuBar(UIExtraDataMetaDefs::MenuType type) const;
    /** Defines menu-bar @a restriction for passed @a level. */
    void setRestrictionForMenuBar(UIActionRestrictionLevel level, UIExtraDataMetaDefs::MenuType restriction);

#ifdef Q_WS_MAC
    /** Returns whether the action with passed @a type is allowed in the 'Application' menu. */
    bool isAllowedInMenuApplication(UIExtraDataMetaDefs::MenuApplicationActionType type) const;
    /** Defines 'Application' menu @a restriction for passed @a level. */
    void setRestrictionForMenuApplication(UIActionRestrictionLevel level, UIExtraDataMetaDefs::MenuApplicationActionType restriction);
#endif /* Q_WS_MAC */

    /** Returns whether the action with passed @a type is allowed in the 'Help' menu. */
    bool isAllowedInMenuHelp(UIExtraDataMetaDefs::MenuHelpActionType type) const;
    /** Defines 'Help' menu @a restriction for passed @a level. */
    void setRestrictionForMenuHelp(UIActionRestrictionLevel level, UIExtraDataMetaDefs::MenuHelpActionType restriction);

    /** Hot-key processing delegate. */
    bool processHotKey(const QKeySequence &key);

    /** Returns extra-data ID to save keyboard shortcuts under. */
    virtual QString shortcutsExtraDataID() const = 0;

    /** Returns the list of main menus. */
    virtual QList<QMenu*> menus() const = 0;

protected slots:

    /** Handles menu prepare. */
    void sltHandleMenuPrepare();

    /** Loads keyboard shortcuts of action-pool into shortcuts-pool. */
    void sltApplyShortcuts() { updateShortcuts(); }

protected:

    /** Constructor of the action-pool of passed @a type. */
    UIActionPool(UIActionPoolType type, bool fTemporary = false);

    /** Prepare routine. */
    void prepare();
    /** Prepare pool routine. */
    virtual void preparePool();
    /** Prepare connections routine. */
    virtual void prepareConnections();
    /** Cleanup connections routine. */
    virtual void cleanupConnections() {}
    /** Cleanup pool routine. */
    virtual void cleanupPool();
    /** Cleanup routine. */
    void cleanup();

    /** Update configuration routine. */
    virtual void updateConfiguration();

    /** Update menu routine. */
    virtual void updateMenu(int iIndex);
    /** Update menus routine. */
    virtual void updateMenus() = 0;
#ifdef RT_OS_DARWIN
    /** Update 'Application' menu routine. */
    virtual void updateMenuApplication();
#endif /* RT_OS_DARWIN */
    /** Update 'Help' menu routine. */
    virtual void updateMenuHelp();

    /** Update shortcuts. */
    virtual void updateShortcuts();

    /** Translation handler. */
    virtual void retranslateUi();

    /** General event handler. */
    virtual bool event(QEvent *pEvent);

    /** Adds action into corresponding menu. */
    bool addAction(UIMenu *pMenu, UIAction *pAction, bool fReallyAdd = true);
    /** Adds action's menu into corresponding menu list. */
    bool addMenu(QList<QMenu*> &menuList, UIAction *pAction, bool fReallyAdd = true);

    /** Holds the action-pool type. */
    const UIActionPoolType m_type;
    /** Holds whether this action-pool is temporary. */
    const bool m_fTemporary;

    /** Holds the map of actions. */
    QMap<int, UIAction*> m_pool;
    /** Holds the map of validation handlers. */
    QMap<int, PointerToFunction> m_menuUpdateHandlers;
    /** Holds the set of invalidated action indexes. */
    QSet<int> m_invalidations;

    /** Holds restricted menu types. */
    QMap<UIActionRestrictionLevel, UIExtraDataMetaDefs::MenuType> m_restrictedMenus;
#ifdef Q_WS_MAC
    /** Holds restricted action types of the Application menu. */
    QMap<UIActionRestrictionLevel, UIExtraDataMetaDefs::MenuApplicationActionType> m_restrictedActionsMenuApplication;
#endif /* Q_WS_MAC */
    /** Holds restricted action types of the Help menu. */
    QMap<UIActionRestrictionLevel, UIExtraDataMetaDefs::MenuHelpActionType> m_restrictedActionsMenuHelp;
};

#endif /* !___UIActionPool_h___ */
