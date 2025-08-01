#include "classify.hpp"

namespace tlc::lexeme {
    const HashMap<StrV, Lexeme> nonTypeKeywordTable = {
        /* Visibility */{module.str(), module}, {import_.str(), import_},
        {export_.str(), export_}, {internal.str(), internal},
        {local.str(), local}, {extern_.str(), extern_},
        /* Definition */{let.str(), let}, {fn.str(), fn}, {trait.str(), trait},
        {type.str(), type}, {enum_.str(), enum_}, {flag.str(), flag},
        /* Control */{for_.str(), for_}, {return_.str(), return_},
        {yield.str(), yield}, {match.str(), match}, {defer.str(), defer},
        {preface.str(), preface},
        /* Adverb */{by.str(), by}, {of.str(), of}, {in.str(), in},
        {when.str(), when},
        /* Boolean */{true_.str(), true_}, {false_.str(), false_},
        /* Object */{self.str(), self}, {pub.str(), pub}, {prv.str(), prv},
        {impl.str(), impl},
    };

    const HashMap<StrV, Lexeme> operatorTable = {
        /* Triple characters */
        {greater2Equal.str(), greater2Equal},
        {less2Equal.str(), less2Equal},
        {star2Equal.str(), star2Equal},
        {dot3.str(), dot3},

        /* Double characters */
        {exclaimEqual.str(), exclaimEqual},
        {starEqual.str(), starEqual},
        {ampersandEqual.str(), ampersandEqual},
        {barEqual.str(), barEqual},
        {hatEqual.str(), hatEqual},
        {fwdSlashEqual.str(), fwdSlashEqual},
        {percentEqual.str(), percentEqual},
        {greaterEqual.str(), greaterEqual},
        {lessEqual.str(), lessEqual},
        {plusEqual.str(), plusEqual},
        {minusEqual.str(), minusEqual},
        {barGreater.str(), barGreater},
        {minusGreater.str(), minusGreater},
        {equalGreater.str(), equalGreater},
        {colon2.str(), colon2},
        {star2.str(), star2},
        {ampersand2.str(), ampersand2},
        {bar2.str(), bar2},
        {plus2.str(), plus2},
        {minus2.str(), minus2},
        {equal2.str(), equal2},
        {greater2.str(), greater2},
        {less2.str(), less2},
        {qMark2.str(), qMark2},
        {dot2.str(), dot2},

        /* Single character */
        {leftParen.str(), leftParen},
        {rightParen.str(), rightParen},
        {leftBracket.str(), leftBracket},
        {rightBracket.str(), rightBracket},
        {leftBrace.str(), leftBrace},
        {rightBrace.str(), rightBrace},
        {dot.str(), dot},
        {comma.str(), comma},
        {colon.str(), colon},
        {semicolon.str(), semicolon},
        {star.str(), star},
        {ampersand.str(), ampersand},
        {hash.str(), hash},
        {bar.str(), bar},
        {hat.str(), hat},
        {plus.str(), plus},
        {minus.str(), minus},
        {fwdSlash.str(), fwdSlash},
        {percent.str(), percent},
        {exclaim.str(), exclaim},
        {equal.str(), equal},
        {greater.str(), greater},
        {less.str(), less},
        {sQuote.str(), sQuote},
        {dQuote.str(), dQuote},
        {qMark.str(), qMark},
        {tilde.str(), tilde},
        {dollar.str(), dollar},
        {at.str(), at},
    };

    const HashSet<StrV> fundamentalTypes = {
        "Int", "Float", "Bool", "Char", "Void", "String", "Any", "Opt",
    };

    const OpGraph3 opGraph = [] {
        OpGraph3 graph;
        rng::for_each(
            operatorTable
            | rv::transform([](auto const& entry) {
                return entry.first;
            }), [&graph](StrV const op) {
                if (!graph.contains(op[0])) {
                    graph[op[0]] = {};
                }
                if (op.length() > 1) {
                    if (!graph[op[0]].contains(op[1])) {
                        graph[op[0]][op[1]] = {};
                    }
                    if (op.length() > 2) {
                        graph[op[0]][op[1]].emplace(op[2]);
                    }
                }
            });
        return graph;
    }();

    // {":=>", EToken::ColonEqualGreater},
    // {":~>", EToken::ColonTildeGreater},
    // {"!!", EToken::Exclaim2},
    // {"<-", EToken::LessMinus},
}
