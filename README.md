## Workflow

<ul>
    <li>
        Use this guideline when before adding tags to a test case:
        <ul>
            <li>
                If the test case belongs to a root module (lex, parse, syntax, etc), the first tag 
                should be the name of module. Hence, each test case's first tag will be the name of 
                the root module to which they associate. For example, "[Lex]", "[Parse]", etc.
            </li>
            <li>
                Add an additional submodule tag if the test case is not directly and logically for 
                the "intended" public-interface classes or functions of its current root module. 
                For example, test cases for parse::match and parse::any are not for parse::Parser, 
                at least directly. Thus, those test cases have two tags: "[Parse][ParserCombinators]".
            </li>
            <li>
                If a test case is created due to an issue, the test case's last tag must be "[Issue]". 
                When the issue has been resolved, i.e. the test case passes, replace the tag 
                "[Issue]" with "[Fixed]" and add a comment before the test case body to describe the 
                problem. For example,
                /**
                 * Fixed on mm/dd/yy.
                 * Attempts: 
                 * Solution: 
                 */
            </li>
        </ul> 
    </li>
</ul>
