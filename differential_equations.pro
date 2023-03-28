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
    analitical_geometry/polyhedrondefinitioncomponent.cpp \
    commandresponse.cpp \
    expressions_drawer/abstract_big_parent_expression.cpp \
    expressions_drawer/abstract_expression.cpp \
    expressions_drawer/abstract_parent_expression.cpp \
    expressions_drawer/abstract_twin_parent_expression.cpp \
    expressions_drawer/argument_expression.cpp \
    expressions_drawer/asterix_expression.cpp \
    expressions_drawer/at_value_expression.cpp \
    expressions_drawer/base_expression.cpp \
    expressions_drawer/bracketed_expression.cpp \
    expressions_drawer/cap_expression.cpp \
    expressions_drawer/case_expression.cpp \
    expressions_drawer/chain_expression.cpp \
    expressions_drawer/character_expression.cpp \
    expressions_drawer/circulation_expression.cpp \
    expressions_drawer/comma_expression.cpp \
    expressions_drawer/common_function_expression.cpp \
    expressions_drawer/custom_text_expression.cpp \
    expressions_drawer/empty_expression.cpp \
    expressions_drawer/expression_builder.cpp \
    expressions_drawer/extended_number_expresssion.cpp \
    expressions_drawer/fraction_expression.cpp \
    expressions_drawer/frame_expression.cpp \
    expressions_drawer/function_expression.cpp \
    expressions_drawer/function_name_expression.cpp \
    expressions_drawer/group_expression.cpp \
    expressions_drawer/index_expression.cpp \
    expressions_drawer/integral_expression.cpp \
    expressions_drawer/lambda_expression.cpp \
    expressions_drawer/limit_expression.cpp \
    expressions_drawer/matrix_expression.cpp \
    expressions_drawer/nabla_expression.cpp \
    expressions_drawer/null_expression.cpp \
    expressions_drawer/number_expresssion.cpp \
    expressions_drawer/planck_expression.cpp \
    expressions_drawer/product_expression.cpp \
    expressions_drawer/root_expression.cpp \
    expressions_drawer/round_bracketed_expression.cpp \
    expressions_drawer/sign_expression.cpp \
    expressions_drawer/simple_expression.cpp \
    expressions_drawer/space_expression.cpp \
    expressions_drawer/stand_expression.cpp \
    expressions_drawer/strokes_expression.cpp \
    expressions_drawer/summa_expression.cpp \
    expressions_drawer/variable_expression.cpp \
    form.cpp \
    functioninterpretationtoken.cpp \
    functionliteral.cpp \
    linevalue.cpp \
    main.cpp \
    mainapplicationspace.cpp \
    mainwindow.cpp \
    mathexpression.cpp \
    matrixvalue.cpp \
    parser.cpp \
    physics/constantsmanager.cpp \
    physics/interruptingevent.cpp \
    physics/phmodel.cpp \
    physics/phobject.cpp \
    physics/stageresultdescriptor.cpp \
    physics/vectorfunction.cpp \
    planevalue.cpp \
    polygonvalue.cpp \
    polyhedronvalue.cpp \
    qimageedit.cpp \
    script.cpp \
    scriptsnamespace.cpp \
    scripttextfield.cpp \
    showstepsofdifursform.cpp \
    stringvalue.cpp \
    tests.cpp \
    variableliteral.cpp \
    vectorvalue.cpp

HEADERS += \
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
    analitical_geometry/polyhedrondefinitioncomponent.h \
    commandresponse.h \
    expressions_drawer/abstract_big_parent_expression.h \
    expressions_drawer/abstract_expression.h \
    expressions_drawer/abstract_parent_expression.h \
    expressions_drawer/abstract_twin_parent_expression.h \
    expressions_drawer/argument_expression.h \
    expressions_drawer/asterix_expression.h \
    expressions_drawer/at_value_expression.h \
    expressions_drawer/base_expression.h \
    expressions_drawer/bracketed_expression.h \
    expressions_drawer/cap_expression.h \
    expressions_drawer/case_expression.h \
    expressions_drawer/chain_expression.h \
    expressions_drawer/character_expression.h \
    expressions_drawer/circulation_expression.h \
    expressions_drawer/comma_expression.h \
    expressions_drawer/common_function_expression.h \
    expressions_drawer/custom_text_expression.h \
    expressions_drawer/empty_expression.h \
    expressions_drawer/expression_builder.h \
    expressions_drawer/extended_number_expresssion.h \
    expressions_drawer/fraction_expression.h \
    expressions_drawer/frame_expression.h \
    expressions_drawer/function_expression.h \
    expressions_drawer/function_name_expression.h \
    expressions_drawer/global.h \
    expressions_drawer/group_expression.h \
    expressions_drawer/index_expression.h \
    expressions_drawer/integral_expression.h \
    expressions_drawer/lambda_expression.h \
    expressions_drawer/limit_expression.h \
    expressions_drawer/matrix_expression.h \
    expressions_drawer/nabla_expression.h \
    expressions_drawer/null_expression.h \
    expressions_drawer/number_expresssion.h \
    expressions_drawer/planck_expression.h \
    expressions_drawer/product_expression.h \
    expressions_drawer/root_expression.h \
    expressions_drawer/round_bracketed_expression.h \
    expressions_drawer/sign_expression.h \
    expressions_drawer/simple_expression.h \
    expressions_drawer/space_expression.h \
    expressions_drawer/stand_expression.h \
    expressions_drawer/strokes_expression.h \
    expressions_drawer/summa_expression.h \
    expressions_drawer/variable_expression.h \
    form.h \
    functioninterpretationtoken.h \
    functionliteral.h \
    linevalue.h \
    mainapplicationspace.h \
    mainwindow.h \
    mathexpression.h \
    matrixvalue.h \
    parser.h \
    physics/constantsmanager.h \
    physics/interruptingevent.h \
    physics/phmodel.h \
    physics/phobject.h \
    physics/stageresultdescriptor.h \
    physics/vectorfunction.h \
    planevalue.h \
    polygonvalue.h \
    polyhedronvalue.h \
    qimageedit.h \
    script.h \
    scriptsnamespace.h \
    scripttextfield.h \
    showstepsofdifursform.h \
    stringvalue.h \
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
    algebra/algebra_mod.qmodel \
    algebra/algebra_modul_behavior.qmodel \ \
    difurs_examples \
    expressions_drawer/math_expressions.pri

