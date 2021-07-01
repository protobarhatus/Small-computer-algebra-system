QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += c++1z
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../../mo2/Balloon/balloontip.cpp \
    ../../mo2/ExprDraw/ExtSymbol/texpr_comma.cpp \
    ../../mo2/ExprDraw/ExtSymbol/texpr_extsymbol.cpp \
    ../../mo2/ExprDraw/ExtSymbol/texpr_lambda.cpp \
    ../../mo2/ExprDraw/ExtSymbol/texpr_nabla.cpp \
    ../../mo2/ExprDraw/ExtSymbol/texpr_plank.cpp \
    ../../mo2/ExprDraw/ExtSymbol/texpr_sign.cpp \
    ../../mo2/ExprDraw/Number/texpr_expnumber.cpp \
    ../../mo2/ExprDraw/Number/texpr_number.cpp \
    ../../mo2/ExprDraw/Parent/BigParent/CommonFunc/texpr_commonfunc.cpp \
    ../../mo2/ExprDraw/Parent/BigParent/CommonFunc/texpr_func.cpp \
    ../../mo2/ExprDraw/Parent/BigParent/texpr_atvalue.cpp \
    ../../mo2/ExprDraw/Parent/BigParent/texpr_bigparent.cpp \
    ../../mo2/ExprDraw/Parent/BigParent/texpr_ratio.cpp \
    ../../mo2/ExprDraw/Parent/BigParent/texpr_root.cpp \
    ../../mo2/ExprDraw/Parent/Chain/Bracketed/texpr_argument.cpp \
    ../../mo2/ExprDraw/Parent/Chain/Bracketed/texpr_base.cpp \
    ../../mo2/ExprDraw/Parent/Chain/Bracketed/texpr_bracketed.cpp \
    ../../mo2/ExprDraw/Parent/Chain/Bracketed/texpr_round.cpp \
    ../../mo2/ExprDraw/Parent/Chain/texpr_chain.cpp \
    ../../mo2/ExprDraw/Parent/TwinParent/GroupOp/texpr_circ.cpp \
    ../../mo2/ExprDraw/Parent/TwinParent/GroupOp/texpr_groupop.cpp \
    ../../mo2/ExprDraw/Parent/TwinParent/GroupOp/texpr_integral.cpp \
    ../../mo2/ExprDraw/Parent/TwinParent/GroupOp/texpr_prod.cpp \
    ../../mo2/ExprDraw/Parent/TwinParent/GroupOp/texpr_summa.cpp \
    ../../mo2/ExprDraw/Parent/TwinParent/texpr_index.cpp \
    ../../mo2/ExprDraw/Parent/TwinParent/texpr_twinparent.cpp \
    ../../mo2/ExprDraw/Parent/texpr_cap.cpp \
    ../../mo2/ExprDraw/Parent/texpr_case.cpp \
    ../../mo2/ExprDraw/Parent/texpr_lim.cpp \
    ../../mo2/ExprDraw/Parent/texpr_matrix.cpp \
    ../../mo2/ExprDraw/Parent/texpr_parent.cpp \
    ../../mo2/ExprDraw/Parent/texpr_stand.cpp \
    ../../mo2/ExprDraw/Simple/texpr_asterix.cpp \
    ../../mo2/ExprDraw/Simple/texpr_customtext.cpp \
    ../../mo2/ExprDraw/Simple/texpr_funcname.cpp \
    ../../mo2/ExprDraw/Simple/texpr_simple.cpp \
    ../../mo2/ExprDraw/Simple/texpr_var.cpp \
    ../../mo2/ExprDraw/texpr_class.cpp \
    ../../mo2/ExprDraw/texpr_empty.cpp \
    ../../mo2/ExprDraw/texpr_space.cpp \
    ../../mo2/ExprDraw/texpr_strokes.cpp \
    ../../mo2/ExprMake/texpr_builder.cpp \
    ../../mo2/ExprMake/texpr_error.cpp \
    ../../mo2/exprguidepaintwidget.cpp \
    ../../mo2/exprguidewidget.cpp \
    ../../mo2/testexprmakewidget.cpp \
    ../../mo2/testpaintwidget.cpp \
    abstractvalue.cpp \
    algebra/Matrix.inl \
    algebra/Vector.inl \
    algebra/Vector2.cpp \
    algebra/Vector3.cpp \
    algebra/absexmemorychecker.cpp \
    algebra/absolutevalue.cpp \
    algebra/abstractexpression.cpp \
    algebra/algexpr.cpp \
    algebra/arcsinus.cpp \
    algebra/arctangent.cpp \
    algebra/constant.cpp \
    algebra/cosinus.cpp \
    algebra/cotangent.cpp \
    algebra/degree.cpp \
    algebra/derivativeobject.cpp \
    algebra/difequationrootconditions.cpp \
    algebra/differential.cpp \
    algebra/equationrootsconditions.cpp \
    algebra/exception.cpp \
    algebra/factorization.cpp \
    algebra/fractal.cpp \
    algebra/functionrange.cpp \
    algebra/functionrangesegment.cpp \
    algebra/galuafield.cpp \
    algebra/logarithm.cpp \
    algebra/mathsets.cpp \
    algebra/number.cpp \
    algebra/numberpolynom.cpp \
    algebra/polynom.cpp \
    algebra/polynomial.cpp \
    algebra/polynomials_factorization.cpp \
    algebra/primetest.cpp \
    algebra/sinus.cpp \
    algebra/solving_differential_equations.cpp \
    algebra/solving_equations.cpp \
    algebra/some_algebra_expression_conversions.cpp \
    algebra/tangent.cpp \
    algebra/trigonometrical_conversions.cpp \
    algebra/variable.cpp \
    algebra/variablesdefinition.cpp \
    algebra/variablesdistributor.cpp \
    algebra/variablesnamedistributor.cpp \
    algebraicexpressionvalue.cpp \
    analitical_geometry/geometry_3d.cpp \
    analitical_geometry/line3d.cpp \
    analitical_geometry/plane.cpp \
    analitical_geometry/polyhedron.cpp \
    commandresponse.cpp \
    form.cpp \
    functioninterpretationtoken.cpp \
    functionliteral.cpp \
    main.cpp \
    mainapplicationspace.cpp \
    mainwindow.cpp \
    mathexpression.cpp \
    matrixvalue.cpp \
    parser.cpp \
    script.cpp \
    scriptsnamespace.cpp \
    scripttextfield.cpp \
    showstepsofdifursform.cpp \
    tests.cpp \
    variableliteral.cpp \
    vectorvalue.cpp

HEADERS += \
    ../../mo2/Balloon/balloontip.h \
    ../../mo2/ExprDraw/ExtSymbol/texpr_comma.h \
    ../../mo2/ExprDraw/ExtSymbol/texpr_extsymbol.h \
    ../../mo2/ExprDraw/ExtSymbol/texpr_lambda.h \
    ../../mo2/ExprDraw/ExtSymbol/texpr_nabla.h \
    ../../mo2/ExprDraw/ExtSymbol/texpr_plank.h \
    ../../mo2/ExprDraw/ExtSymbol/texpr_sign.h \
    ../../mo2/ExprDraw/Number/texpr_expnumber.h \
    ../../mo2/ExprDraw/Number/texpr_number.h \
    ../../mo2/ExprDraw/Parent/BigParent/CommonFunc/texpr_commonfunc.h \
    ../../mo2/ExprDraw/Parent/BigParent/CommonFunc/texpr_func.h \
    ../../mo2/ExprDraw/Parent/BigParent/texpr_atvalue.h \
    ../../mo2/ExprDraw/Parent/BigParent/texpr_bigparent.h \
    ../../mo2/ExprDraw/Parent/BigParent/texpr_ratio.h \
    ../../mo2/ExprDraw/Parent/BigParent/texpr_root.h \
    ../../mo2/ExprDraw/Parent/Chain/Bracketed/texpr_argument.h \
    ../../mo2/ExprDraw/Parent/Chain/Bracketed/texpr_base.h \
    ../../mo2/ExprDraw/Parent/Chain/Bracketed/texpr_bracketed.h \
    ../../mo2/ExprDraw/Parent/Chain/Bracketed/texpr_round.h \
    ../../mo2/ExprDraw/Parent/Chain/texpr_chain.h \
    ../../mo2/ExprDraw/Parent/TwinParent/GroupOp/texpr_circ.h \
    ../../mo2/ExprDraw/Parent/TwinParent/GroupOp/texpr_groupop.h \
    ../../mo2/ExprDraw/Parent/TwinParent/GroupOp/texpr_integral.h \
    ../../mo2/ExprDraw/Parent/TwinParent/GroupOp/texpr_prod.h \
    ../../mo2/ExprDraw/Parent/TwinParent/GroupOp/texpr_summa.h \
    ../../mo2/ExprDraw/Parent/TwinParent/texpr_index.h \
    ../../mo2/ExprDraw/Parent/TwinParent/texpr_twinparent.h \
    ../../mo2/ExprDraw/Parent/texpr_cap.h \
    ../../mo2/ExprDraw/Parent/texpr_case.h \
    ../../mo2/ExprDraw/Parent/texpr_lim.h \
    ../../mo2/ExprDraw/Parent/texpr_matrix.h \
    ../../mo2/ExprDraw/Parent/texpr_parent.h \
    ../../mo2/ExprDraw/Parent/texpr_stand.h \
    ../../mo2/ExprDraw/Simple/texpr_asterix.h \
    ../../mo2/ExprDraw/Simple/texpr_customtext.h \
    ../../mo2/ExprDraw/Simple/texpr_funcname.h \
    ../../mo2/ExprDraw/Simple/texpr_simple.h \
    ../../mo2/ExprDraw/Simple/texpr_var.h \
    ../../mo2/ExprDraw/expr_draw.h \
    ../../mo2/ExprDraw/expr_draw_pro.h \
    ../../mo2/ExprDraw/texpr_class.h \
    ../../mo2/ExprDraw/texpr_empty.h \
    ../../mo2/ExprDraw/texpr_space.h \
    ../../mo2/ExprDraw/texpr_strokes.h \
    ../../mo2/ExprMake/texpr_builder.h \
    ../../mo2/ExprMake/texpr_error.h \
    ../../mo2/exprguidepaintwidget.h \
    ../../mo2/exprguidewidget.h \
    ../../mo2/testexprmakewidget.h \
    ../../mo2/testpaintwidget.h \
    abstractvalue.h \
    algebra/Matrix.h \
    algebra/Vector.h \
    algebra/Vector2.h \
    algebra/Vector3.h \
    algebra/absexmemorychecker.h \
    algebra/absolutevalue.h \
    algebra/abstractexpression.h \
    algebra/algexpr.h \
    algebra/arcsinus.h \
    algebra/arctangent.h \
    algebra/constant.h \
    algebra/cosinus.h \
    algebra/cotangent.h \
    algebra/degree.h \
    algebra/derivativeobject.h \
    algebra/difequationrootconditions.h \
    algebra/differential.h \
    algebra/equationrootsconditions.h \
    algebra/exception.h \
    algebra/factorization.h \
    algebra/fractal.h \
    algebra/functionrange.h \
    algebra/functionrangesegment.h \
    algebra/galuafield.h \
    algebra/logarithm.h \
    algebra/mathsets.h \
    algebra/number.h \
    algebra/numberpolynom.h \
    algebra/polynom.h \
    algebra/polynomial.h \
    algebra/polynomials_factorization.h \
    algebra/primetest.h \
    algebra/sinus.h \
    algebra/solving_differential_equations.h \
    algebra/solving_equations.h \
    algebra/some_algebra_expression_conversions.h \
    algebra/tangent.h \
    algebra/trigonometrical_conversions.h \
    algebra/variable.h \
    algebra/variablesdefinition.h \
    algebra/variablesdistributor.h \
    algebra/variablesnamedistributor.h \
    algebraicexpressionvalue.h \
    analitical_geometry/geometry_3d.h \
    analitical_geometry/line3d.h \
    analitical_geometry/plane.h \
    analitical_geometry/polyhedron.h \
    commandresponse.h \
    form.h \
    functioninterpretationtoken.h \
    functionliteral.h \
    mainapplicationspace.h \
    mainwindow.h \
    mathexpression.h \
    matrixvalue.h \
    parser.h \
    script.h \
    scriptsnamespace.h \
    scripttextfield.h \
    showstepsofdifursform.h \
    tests.h \
    variableliteral.h \
    vectorvalue.h

FORMS += \
    form.ui \
    mainwindow.ui \
    showstepsofdifursform.ui

TRANSLATIONS += \
    differential_equations_ru_RU.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../../mo2/ExprDraw/ExprDraw.pri \
    ../../mo2/ExprGuide.pro.user \
    ../../mo2/ExprMake/ExprMake.pri \
    ../../mo2/icon.rc \
    ../../mo2/icon.svg \
    algebra/algebra_mod.qmodel \
    algebra/algebra_modul_behavior.qmodel \

SUBDIRS += \
    ../../mo2/ExprGuide.pro
