#include <string>
#include <memory>
#include <iostream>
#include <filesystem>

#include <osl/file.hxx>
#include <rtl/process.h>
#include <cppuhelper/bootstrap.hxx>
#include <com/sun/star/frame/Desktop.hpp>
#include <com/sun/star/frame/XStorable.hpp>
#include <com/sun/star/util/XCloseable.hpp>
#include <com/sun/star/beans/XPropertySet.hpp>
#include <com/sun/star/frame/XComponentLoader.hpp>
#include <com/sun/star/lang/XMultiComponentFactory.hpp>
#include <com/sun/star/presentation/XPresentation.hpp>

#include <cmdline.h>

#include <Windows.h>

using css::uno::Reference;
using css::uno::XComponentContext;
using css::frame::XDesktop;
using css::frame::XComponentLoader;
using css::uno::Sequence;
using css::beans::PropertyValue;
using css::frame::XStorable;
using css::util::XCloseable;

rtl::OUString create_from_chars(const char* chars)
{
    int size = MultiByteToWideChar(CP_ACP, 0, chars, -1, NULL, 0);
    auto wchars = std::make_unique<WCHAR[]>(size);
    MultiByteToWideChar(CP_ACP, 0, chars, -1, (LPWSTR)wchars.get(), size);
    return rtl::OUString(wchars.get());
}

int main(int argc, char* argv[]) try
{
    cmdline::parser prsr;

    prsr.add<std::string>("infile", 'i', "input file. (e.g. C:/in.pptx or C:\\in.pptx)", true, "");
    prsr.add<std::string>("outfile", 'o', "output file. (e.g. C:/out.pdf or C:\\out.pdf)", true, "");
    prsr.add<std::string>("password", 'p', "password of document (if need)", false, "");

    prsr.parse_check(argc, argv);
    
    rtl::OUString ustrInputSystemPath = create_from_chars(prsr.get<std::string>("infile").c_str());
    rtl::OUString ustrOutputSystemPath = create_from_chars(prsr.get<std::string>("outfile").c_str());
    rtl::OUString ustrOutputUrl, ustrInputUrl;
    osl::FileBase::getFileURLFromSystemPath(ustrInputSystemPath, ustrInputUrl);
    osl::FileBase::getFileURLFromSystemPath(ustrOutputSystemPath, ustrOutputUrl);

    Reference<XComponentContext> xComponentContext = cppu::bootstrap();
    auto xMultiComponentFactory = xComponentContext->getServiceManager();

    Reference<XDesktop> xDesktop(xMultiComponentFactory->createInstanceWithContext(
                                                             "com.sun.star.frame.Desktop",
                                                             xComponentContext),
                                 css::uno::UNO_QUERY);
    if (!xDesktop.is())
    {
        std::cout << "XDesktop could not instantiate\n";
        return EXIT_FAILURE;
    }

    Reference<XComponentLoader> xComponentLoader(xDesktop, css::uno::UNO_QUERY_THROW);
    if (!xComponentLoader.is())
    {
        std::cout << "XComponentLoader could not instantiate\n";
        return EXIT_FAILURE;
    }

    Sequence<PropertyValue> loadProperties(1);
    loadProperties[0].Name = "Hidden";
    loadProperties[0].Value <<= true;
    auto xComponent = xComponentLoader->loadComponentFromURL(
                                            ustrInputUrl,
                                            "_blank",
                                            sal_Int32(0),
                                            loadProperties);

    std::cout << std::filesystem::path(prsr.get<std::string>("infile")) << " has been loaded\n";

    Reference<XStorable> xStorable(xComponent, css::uno::UNO_QUERY_THROW);
    Sequence<PropertyValue> storeProps(3);
    storeProps[0].Name = "FilterName";
    storeProps[0].Value <<= rtl::OUString("impress_pdf_Export");
    storeProps[1].Name = "Overwrite";
    storeProps[1].Value <<= true;
    storeProps[2].Name = "SelectPdfVersion";
    storeProps[2].Value <<= sal_Int32(1);
    xStorable->storeToURL(ustrOutputUrl, storeProps);

    std::cout << "conversion done\n";
    std::cout << "file has been written in " << std::filesystem::path(prsr.get<std::string>("outfile")) << '\n';

    css::uno::Reference<css::util::XCloseable> xCloseable(xComponent, css::uno::UNO_QUERY_THROW);
    xCloseable->close(false);

    return EXIT_SUCCESS;
}
catch (css::uno::Exception& e)
{
    std::cout << e.Message << std::endl;
    return EXIT_FAILURE;
}
