/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the qmake application of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MSVC_VCPROJ_H
#define MSVC_VCPROJ_H

#include "winmakefile.h"
#include "msvc_objectmodel.h"

QT_BEGIN_NAMESPACE

enum Target {
    Application,
    SharedLib,
    StaticLib
};

class QUuid;
struct VcsolutionDepend;
class VcprojGenerator : public Win32MakefileGenerator
{
    bool is64Bit;
    bool writeVcprojParts(QTextStream &);

    bool writeMakefile(QTextStream &);
    bool writeProjectMakefile();

    void init();

public:
    VcprojGenerator();
    ~VcprojGenerator();

    QString defaultMakefile() const;
    QString precompH, precompHFilename, precompCPP,
            precompObj, precompPch;
    bool autogenPrecompCPP;
    static bool hasBuiltinCompiler(const QString &file);

    QHash<QString, QStringList> extraCompilerSources;
    QHash<QString, QStringList> extraCompilerOutputs;
    bool usePCH;
    VCProjectWriter *projectWriter;

protected:
    virtual VCProjectWriter *createProjectWriter();
    virtual bool doDepends() const { return false; } //never necesary
    virtual void processSources() { filterIncludedFiles("SOURCES"); filterIncludedFiles("GENERATED_SOURCES"); }
    using MakefileGenerator::ReplaceFor;
    virtual QString replaceExtraCompilerVariables(const QString &, const QStringList &, const QStringList &, ReplaceFor);
    inline QString replaceExtraCompilerVariables(const QString &val, const QString &in, const QString &out, ReplaceFor forShell)
    { return MakefileGenerator::replaceExtraCompilerVariables(val, in, out, forShell); }
    virtual bool supportsMetaBuild() { return true; }
    virtual bool supportsMergedBuilds() { return true; }
    virtual bool mergeBuildProject(MakefileGenerator *other);

    virtual bool openOutput(QFile &file, const QString &build) const;

    virtual void initProject();
    void initConfiguration();
    void initCompilerTool();
    void initLinkerTool();
    void initLibrarianTool();
    void initManifestTool();
    void initResourceTool();
    void initIDLTool();
    void initCustomBuildTool();
    void initPreBuildEventTools();
    void initPostBuildEventTools();
    void initDeploymentTool();
    void initWinDeployQtTool();
    void initPreLinkEventTools();
    void initRootFiles();
    void initSourceFiles();
    void initHeaderFiles();
    void initGeneratedFiles();
    void initTranslationFiles();
    void initFormFiles();
    void initResourceFiles();
    void initDeploymentFiles();
    void initLexYaccFiles();
    void initExtraCompilerOutputs();

    void writeSubDirs(QTextStream &t); // Called from VCXProj backend
    QUuid getProjectUUID(const QString &filename=QString()); // Called from VCXProj backend

    Target projectTarget;

    // Used for single project
    VCProjectSingleConfig vcProject;

    // Holds all configurations for glue (merged) project
    QList<VcprojGenerator*> mergedProjects;

private:
    ProStringList collectDependencies(QMakeProject *proj, QHash<QString, QString> &projLookup,
                                      QHash<QString, QString> &projGuids,
                                      QHash<VcsolutionDepend *, QStringList> &extraSubdirs,
                                      QHash<QString, VcsolutionDepend*> &solution_depends,
                                      QList<VcsolutionDepend*> &solution_cleanup,
                                      QTextStream &t,
                                      QHash<QString, ProStringList> &subdirProjectLookup,
                                      const ProStringList &allDependencies = ProStringList());
    QUuid increaseUUID(const QUuid &id);
    QString retrievePlatformToolSet() const;
    bool isStandardSuffix(const QString &suffix) const;
    friend class VCFilter;
};

inline QString VcprojGenerator::defaultMakefile() const
{
    return project->first("TARGET") + project->first("VCPROJ_EXTENSION");
}

QT_END_NAMESPACE

#endif // MSVC_VCPROJ_H
