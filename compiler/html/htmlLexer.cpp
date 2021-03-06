//
// Created by Theo Bessel on 2019-03-25.
//

#include <unordered_map>
#include <regex>
#include "htmlLexer.hpp"
#include "../../usual/logger.hpp"
#include "../../usual/files.hpp"

HtmlLexer::HtmlLexer() {
    bool lexerInit = false;
    bool lexingSuccess = true;
    if(lexingSuccess){
        lexerInit = true;
    }

    //tokenize(getfile("/Users/theobessel/AtherisWeb/render/mainWindow/"));

    usual::clog("Lexing achieved : ", lexerInit);
}

std::string HtmlLexer::getfile(fs::path path) {
    return usual::readFile(path);
}

void HtmlLexer::tokenize(const std::string &file) {

}
/*
enum TAGS {
    T_COMMENTARY,
    T_DOCTYPE,
    T_A,
    T_ABBR,
    T_ACRONYM,
    T_ADDRESS,

    T_APPLET,
    T_AREA,
    T_ARTICLE,
    T_ASIDE,
    T_AUDIO,
    T_B,
    T_BASE,
    T_BASEFONT,
    T_BDI,
    T_BDO,
    T_BGSOUND,
    T_BIG,
    T_BLINK,
    T_BLOCKQUOTE,
    T_BODY,
    T_BR,
    T_BUTTON,
    T_CANVAS,
    T_CAPTION,
    T_CENTER,
    T_CITE,
    T_CODE,
    T_COL,
    T_COLGROUP,
    T_COMMAND,
    T_CONTENT,
    T_DATA,
    T_DATALIST,
    T_DD,
    T_DEL,
    T_DETAILS,
    T_DFN,
    T_DIALOG,
    T_DIR,
    T_DIV,
    T_DL,
    T_DT,
    T_ELEMENT,
    T_EM,
    T_EMBED,
    T_FIELDSET,
    T_FIGCAPTION,
    T_FIGURE,
    T_FONT,
    T_FOOTER,
    T_FORM,
    T_FRAME,
    T_FRAMESET,
    T_H1,
    T_H2,
    T_H3,
    T_H4,
    T_H5,
    T_H6,
    T_HEAD,
    T_HEADER,
    T_HGROUP,
    T_HR,
    T_HTML,
    T_I,
    T_IFRAME,
    T_IMAGE,
    T_IMG,
    T_INPUT,
    T_INS,
    T_ISINDEX,
    T_KBD,
    T_KEYGEN,
    T_LABEL,
    T_LEGEND,
    T_LI,
    T_LINK,
    T_LISTING,
    T_MAIN,
    T_MAP,
    T_MARK,
    T_MARQUEE,
    T_MENU,
    T_MENUITEM,
    T_META,
    T_METER,
    T_MULTICOL,
    T_NAV,
    T_NEXTID,
    T_NOBR,
    T_NOEMBED,
    T_NOFRAMES,
    T_NOSCRIPT,
    T_OBJECT,
    T_OL,
    T_OPTGROUP,
    T_OPTION,
    T_OUTPUT,
    T_P,
    T_PARAM,
    T_PICTURE,
    T_PLAINTEXT,
    T_PRE,
    T_PROGRESS,
    T_Q,
    T_RB,
    T_RP,
    T_RT,
    T_RTC,
    T_RUBY,
    T_S,
    T_SAMP,
    T_SCRIPT,
    T_SECTION,
    T_SELECT,
    T_SHADOW,
    T_SLOT,
    T_SMALL,
    T_SOURCE,
    T_SPACER,
    T_SPAN,
    T_STRIKE,
    T_STRONG,
    T_STYLE,
    T_SUB,
    T_SUMMARY,
    T_SUP,
    T_TABLE,
    T_TBODY,
    T_TD,
    T_TEMPLATE,
    T_TEXTAREA,
    T_TFOOT,
    T_TH,
    T_THEAD,
    T_TIME,
    T_TITLE,
    T_TR,
    T_TRACK,
    T_TT,
    T_U,
    T_UL,
    T_VAR,
    T_VIDEO,
    T_WBR,
    T_XMP
};

enum ATTRIBUTES {
    A_ACCEPT,
    A_ACCEPTCHARSET,
    A_ACCESSKEY,
    A_ACTION,
    A_ALIGN,
    A_ALLOW,
    A_ALT,
    A_ARCHIVE,
    A_ASYNC,
    A_AUTOCAPITALIZE,
    A_AUTOCOMPLETE,
    A_AUTOFOCUS,
    A_AUTOPLAY,
    A_BGCOLOR,
    A_BORDER,
    A_BUFFERED,
    A_CHALLENGE,
    A_CHARSET,
    A_CHECKED,
    A_CITE,
    A_CLASS,
    A_CODE,
    A_CODEBASE,
    A_COLOR,
    A_COLS,
    A_COLSPAN,
    A_CONTENT,
    A_CONTENTEDITABLE,
    A_CONTEXTMENU,
    A_CONTROLS,
    A_COORDS,
    A_CROSSORIGIN,
    A_CSP,
    A_DATA,
    A_DATAFLD,
    A_DATASRC,
    A_DATAUNIV,
    A_DATETIME,
    A_DEFAULT,
    A_DEFER,
    A_DIR,
    A_DIRNAME,
    A_DISABLED,
    A_DOWNLOAD,
    A_DRAGGABLE,
    A_DROPZONE,
    A_ENCTYPE,
    A_FOR,
    A_FORM,
    A_FORMACTION,
    A_HEADERS,
    A_HEIGHT,
    A_HIDDEN,
    A_HIGH,
    A_HREF,
    A_HREFLANG,
    A_HSPACE,
    A_HTTPEQUIV,
    A_ICON,
    A_ID,
    A_IMPORTANCE,
    A_INTEGRITY,
    A_ISMAP,
    A_ITEMPROP,
    A_KEYTYPE,
    A_KIND,
    A_LABEL,
    A_LANG,
    A_LANGUAGE,
    A_LAZYLOAD,
    A_LIST,
    A_LOOP,
    A_LOW,
    A_MANIFEST,
    A_MAX,
    A_MAYSCRIPT,
    A_MAXLENGTH,
    A_MINLENGTH,
    A_MEDIA,
    A_METHOD,
    A_MIN,
    A_MULTIPLE,
    A_MUTED,
    A_NAME,
    A_NOHREF,
    A_NOVALIDATE,
    A_OBJECT,
    A_OPEN,
    A_OPTIMUM,
    A_PATTERN,
    A_PING,
    A_PLACEHOLDER,
    A_POSTER,
    A_PRELOAD,
    A_RADIOGROUP,
    A_READONLY,
    A_REFERRERPOLICY,
    A_REL,
    A_REV,
    A_REQUIERED,
    A_REVERSED,
    A_ROWS,
    A_ROWSPAN,
    A_SANDBOX,
    A_SCOPE,
    A_SCOPED,
    A_SELECTED,
    A_SHAPE,
    A_SIZE,
    A_SIZES,
    A_SLOT,
    A_SPAN,
    A_SPELLCHECK,
    A_SRC,
    A_SRCDOC,
    A_SRCLANG,
    A_SRCSET,
    A_START,
    A_STEP,
    A_STYLE,
    A_SUMMARY,
    A_TABINDEX,
    A_TARGET,
    A_TITLE,
    A_TRANSLATE,
    A_TYPE,
    A_USEMAP,
    A_VALUE,
    A_VSPACE,
    A_WIDTH,
    A_WRAP
};
*/
/*
std::regex const makeRegex(std::string tag) {
    std::regex const regex {"<" + tag + "[ \\t]*([a-zA-Z]*[ \\t]*=*[ \\t]*\\\"[ \\t]*.*[ \\t]*\\\"[ \\t]*)*>[ \\t]*.*[ \\t]*</" + tag + ">"};
    return regex;
}
*/
//std::regex const makeEmptyTagRegex(std::string balise) {
//    std::regex const regex {"<" + balise + "[ \\t]*([a-zA-Z]*[ \\t]*=*[ \\t]*\\\"[ \\t]*.*[ \\t]*\\\"[ \\t]*)*/*>"};
//    return regex;
//}
/*
std::unordered_map<const std::regex, const TAGS, const std::vector<ATTRIBUTES> > tags {
        {{"<!--"},{"-->"},{TAGS::T_COMMENTARY}},
        {{"<!DOCTYPE "},{">"},{TAGS::T_DOCTYPE}},
        {{makeRegex("a")},{TAGS::T_A},{{ATTRIBUTES::A_HREF},{ATTRIBUTES::A_HREFLANG},{ATTRIBUTES::A_PING},{ATTRIBUTES::A_REFERRERPOLICY},{ATTRIBUTES::A_REL},{ATTRIBUTES::A_TARGET},{ATTRIBUTES::A_TYPE},{ATTRIBUTES::A_CHARSET},{ATTRIBUTES::A_COORDS},{ATTRIBUTES::A_NAME},{ATTRIBUTES::A_REV},{ATTRIBUTES::A_SHAPE}}},
        {{makeRegex("abbr")},{TAGS::T_ABBR}},
        {{makeRegex("acronym")},{TAGS::T_ACRONYM}}, /// obsolete
        {{makeRegex("address")},{TAGS::T_ADDRESS}},
        {{makeRegex("applet")},{TAGS::T_APPLET},{{ATTRIBUTES::A_ALIGN},{ATTRIBUTES::A_ALT},{ATTRIBUTES::A_ARCHIVE},{ATTRIBUTES::A_CODE},{ATTRIBUTES::A_CODEBASE},{ATTRIBUTES::A_DATAFLD},{ATTRIBUTES::A_DATASRC},{ATTRIBUTES::A_HEIGHT},{ATTRIBUTES::A_HSPACE},{ATTRIBUTES::A_MAYSCRIPT},{ATTRIBUTES::A_NAME},{ATTRIBUTES::A_OBJECT},{ATTRIBUTES::A_SRC},{ATTRIBUTES::A_VSPACE},{ATTRIBUTES::A_WIDTH}}}, /// obsolete
        {{makeEmptyTagRegex("area")},{TAGS::T_AREA},{{ATTRIBUTES::A_ALT},{ATTRIBUTES::A_COORDS},{ATTRIBUTES::A_HREF},{ATTRIBUTES::A_HREFLANG},{ATTRIBUTES::A_PING},{ATTRIBUTES::A_REFERRERPOLICY},{ATTRIBUTES::A_REL},{ATTRIBUTES::A_SHAPE},{ATTRIBUTES::A_TARGET},{ATTRIBUTES::A_ACCESSKEY},{ATTRIBUTES::A_NAME},{ATTRIBUTES::A_NOHREF},{ATTRIBUTES::A_TABINDEX},{ATTRIBUTES::A_TYPE}}}, /// empty
        {{makeRegex("article")},{TAGS::T_ARTICLE}},
        {{makeRegex("aside")},{TAGS::T_ASIDE}},
        {{makeRegex("audio")},{TAGS::T_AUDIO},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("b")},{TAGS::T_B},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeEmptyTagRegex("base")},{TAGS::T_BASE},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("basefont")},{TAGS::T_BASEFONT},{{ATTRIBUTES::},{ATTRIBUTES::}}}, /// obsolete
        {{makeRegex("bdi")},{TAGS::T_BDI},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("bdo")},{TAGS::T_BDO},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("bgsound")},{TAGS::T_BGSOUND},{{ATTRIBUTES::},{ATTRIBUTES::}}}, /// obsolete
        {{makeRegex("big")},{TAGS::T_BIG},{{ATTRIBUTES::},{ATTRIBUTES::}}}, /// obsolete
        {{makeRegex("blink")},{TAGS::T_BLINK},{{ATTRIBUTES::},{ATTRIBUTES::}}}, /// obsolete
        {{makeRegex("blockquote")},{TAGS::T_BLOCKQUOTE},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("body")},{TAGS::T_BODY},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeEmptyTagRegex("br")},{TAGS::T_BR},{{ATTRIBUTES::},{ATTRIBUTES::}}}, /// empty
        {{makeRegex("button")},{TAGS::T_BUTTON},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("canvas")},{TAGS::T_CANVAS},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("caption")},{TAGS::T_CAPTION},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("center")},{TAGS::T_CENTER},{{ATTRIBUTES::},{ATTRIBUTES::}}}, /// obsolete
        {{makeRegex("cite")},{TAGS::T_CITE},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("code")},{TAGS::T_CODE},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeEmptyTagRegex("col")},{TAGS::T_COL},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("colgroup")},{TAGS::T_COLGROUP},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("command")},{TAGS::T_COMMAND},{{ATTRIBUTES::},{ATTRIBUTES::}}}, /// obsolete
        {{makeRegex("content")},{TAGS::T_CONTENT},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("data")},{TAGS::T_DATA},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("datalist")},{TAGS::T_DATALIST},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("dd")},{TAGS::T_DD},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("del")},{TAGS::T_DEL},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("details")},{TAGS::T_DETAILS},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("dfn")},{TAGS::T_DFN},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("dialog")},{TAGS::T_DIALOG},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("dir")},{TAGS::T_DIR},{{ATTRIBUTES::},{ATTRIBUTES::}}}, /// obsolete
        {{makeRegex("div")},{TAGS::T_DIV},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("dl")},{TAGS::T_DL},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("dt")},{TAGS::T_DT},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("element")},{TAGS::T_ELEMENT},{{ATTRIBUTES::},{ATTRIBUTES::}}}, /// obsolete
        {{makeRegex("em")},{TAGS::T_EM},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeEmptyTagRegex("embed")},{TAGS::T_EMBED},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("fieldset")},{TAGS::T_FIELDSET},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("figcaption")},{TAGS::T_FIGCAPTION},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("figure")},{TAGS::T_FIGURE},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("font")},{TAGS::T_FONT},{{ATTRIBUTES::},{ATTRIBUTES::}}}, /// obsolete
        {{makeRegex("footer")},{TAGS::T_FOOTER},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("form")},{TAGS::T_FORM},{{ATTRIBUTES::A_ACCEPT},{ATTRIBUTES::A_ACCEPTCHARSET},{ATTRIBUTES::A_ACTION},{ATTRIBUTES::A_AUTOCAPITALIZE},{ATTRIBUTES::A_AUTOCOMPLETE},{ATTRIBUTES::A_ENCTYPE},{ATTRIBUTES::A_METHOD},{ATTRIBUTES::A_NAME},{ATTRIBUTES::A_NOVALIDATE},{ATTRIBUTES::A_TARGET}}},
        {{makeRegex("frame")},{TAGS::T_FRAME},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("frameset")},{TAGS::T_FRAMESET},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("h1")},{TAGS::T_H1},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("h2")},{TAGS::T_H2},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("h3")},{TAGS::T_H3},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("h4")},{TAGS::T_H4},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("h5")},{TAGS::T_H5},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("h6")},{TAGS::T_H6},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("head")},{TAGS::T_HEAD},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("header")},{TAGS::T_HEADER},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("hgroup")},{TAGS::T_HGROUP},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeEmptyTagRegex("hr")},{TAGS::T_HR},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("html")},{TAGS::T_HTML},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("i")},{TAGS::T_I},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("iframe")},{TAGS::T_IFRAME},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("image")},{TAGS::T_IMAGE},{{ATTRIBUTES::},{ATTRIBUTES::}}}, /// obsolete
        {{makeEmptyTagRegex("img")},{TAGS::T_IMG},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeEmptyTagRegex("input")},{TAGS::T_INPUT},{{ATTRIBUTES::A_AUTOCOMPLETE},{ATTRIBUTES::A_AUTOFOCUS},{ATTRIBUTES::A_DISABLED},{ATTRIBUTES::A_FORM},{ATTRIBUTES::A_LIST},{ATTRIBUTES::A_NAME},{ATTRIBUTES::A_READONLY},{ATTRIBUTES::A_REQUIERED},{ATTRIBUTES::A_TABINDEX},{ATTRIBUTES::A_TYPE},{ATTRIBUTES::A_VALUE}}},
        {{makeRegex("ins")},{TAGS::T_INS},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("isindex")},{TAGS::T_ISINDEX},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("kbd")},{TAGS::T_KBD},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeEmptyTagRegex("keygen")},{TAGS::T_KEYGEN},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("label")},{TAGS::T_LABEL},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("legend")},{TAGS::T_LEGEND},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("li")},{TAGS::T_LI},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeEmptyTagRegex("link")},{TAGS::T_LINK},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("listing")},{TAGS::T_LISTING},{{ATTRIBUTES::},{ATTRIBUTES::}}}, /// obsolete
        {{makeRegex("main")},{TAGS::T_MAIN},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("map")},{TAGS::T_MAP},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("mark")},{TAGS::T_MARK},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("marquee")},{TAGS::T_MARQUEE},{{ATTRIBUTES::},{ATTRIBUTES::}}}, /// obsolete
        {{makeRegex("menu")},{TAGS::T_MENU},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("menuitem")},{TAGS::T_MENUITEM},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeEmptyTagRegex("meta")},{TAGS::T_META},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("meter")},{TAGS::T_METER},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("multicol")},{TAGS::T_MULTICOL},{{ATTRIBUTES::},{ATTRIBUTES::}}}, /// obsolete
        {{makeRegex("nav")},{TAGS::T_NAV},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("nextid")},{TAGS::T_NEXTID},{{ATTRIBUTES::},{ATTRIBUTES::}}}, /// obsolete
        {{makeRegex("nobr")},{TAGS::T_NOBR},{{ATTRIBUTES::},{ATTRIBUTES::}}}, /// obsolete
        {{makeRegex("noembed")},{TAGS::T_NOEMBED},{{ATTRIBUTES::},{ATTRIBUTES::}}}, /// obsolete
        {{makeRegex("noframes")},{TAGS::T_NOFRAMES},{{ATTRIBUTES::},{ATTRIBUTES::}}}, /// obsolete
        {{makeRegex("noscript")},{TAGS::T_NOSCRIPT},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("object")},{TAGS::T_OBJECT},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("ol")},{TAGS::T_OL},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("optgroup")},{TAGS::T_OPTGROUP},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("option")},{TAGS::T_OPTION},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("output")},{TAGS::T_OUTPUT},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("p")},{TAGS::T_P},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeEmptyTagRegex("param")},{TAGS::T_PARAM},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("picture")},{TAGS::T_PICTURE},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("plaintext")},{TAGS::T_PLAINTEXT},{{ATTRIBUTES::},{ATTRIBUTES::}}}, /// obsolete
        {{makeRegex("pre")},{TAGS::T_PRE},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("progress")},{TAGS::T_PROGRESS},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("q")},{TAGS::T_Q},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("rb")},{TAGS::T_RB},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("rp")},{TAGS::T_RP},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("rt")},{TAGS::T_RT},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("rtc")},{TAGS::T_RTC},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("ruby")},{TAGS::T_RUBY},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("s")},{TAGS::T_S},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("samp")},{TAGS::T_SAMP},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("script")},{TAGS::T_SCRIPT},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("section")},{TAGS::T_SECTION},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("select")},{TAGS::T_SELECT},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("shadow")},{TAGS::T_SHADOW},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("slot")},{TAGS::T_SLOT},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("small")},{TAGS::T_SMALL},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeEmptyTagRegex("source")},{TAGS::T_SOURCE},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("spacer")},{TAGS::T_SPACER},{{ATTRIBUTES::},{ATTRIBUTES::}}}, /// obsolete
        {{makeRegex("span")},{TAGS::T_SPAN},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("strike")},{TAGS::T_STRIKE},{{ATTRIBUTES::},{ATTRIBUTES::}}}, /// obsolete
        {{makeRegex("strong")},{TAGS::T_STRONG},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("style")},{TAGS::T_STYLE},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("sub")},{TAGS::T_SUB},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("summary")},{TAGS::T_SUMMARY},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("sup")},{TAGS::T_SUP},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("table")},{TAGS::T_TABLE},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("tbody")},{TAGS::T_TBODY},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("td")},{TAGS::T_TD},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("template")},{TAGS::T_TEMPLATE},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("textarea")},{TAGS::T_TEXTAREA},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("tfoot")},{TAGS::T_TFOOT},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("th")},{TAGS::T_TH},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("thead")},{TAGS::T_THEAD},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("time")},{TAGS::T_TIME},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("title")},{TAGS::T_TITLE},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("tr")},{TAGS::T_TR},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeEmptyTagRegex("track")},{TAGS::T_TRACK},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("tt")},{TAGS::T_TT},{{ATTRIBUTES::},{ATTRIBUTES::}}}, /// obsolete
        {{makeRegex("u")},{TAGS::T_U},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("ul")},{TAGS::T_UL},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("var")},{TAGS::T_VAR},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("video")},{TAGS::T_VIDEO},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeEmptyTagRegex("wbr")},{TAGS::T_WBR},{{ATTRIBUTES::},{ATTRIBUTES::}}},
        {{makeRegex("xmp")},{TAGS::T_XMP},{{ATTRIBUTES::},{ATTRIBUTES::}}} /// obsolete
};
*/