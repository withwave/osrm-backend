@routing @testbot @mld
Feature: Multi level routing

    Background:
        Given the profile "testbot"
        And the partition extra arguments "--min-cell-size 4 --small-component-size 1"

    Scenario: Testbot - Multi level routing check partition
        Given the node map
            """
            a───b───e───f
            │   │   │   │
            d───c   h───g
                 ╲ ╱
                  ╳
                 ╱ ╲
            i───j   m───n
            │   │   │   │
            l───k───p───o
            """

        And the ways
            | nodes | highway |
            | abcda | primary |
            | efghe | primary |
            | ijkli | primary |
            | nmop  | primary |
            | cm    | primary |
            | hj    | primary |
            | kp    | primary |
            | be    | primary |

        And the data has been extracted
        When I run "osrm-partition --min-cell-size 4 --small-component-size 1 {processed_file}"
        Then it should exit successfully
        And stdout should not contain "level 1 #cells 1 bit size 1"

    Scenario: Testbot - Multi level routing
        Given the node map
            """
            a───b   e───f
            │   │   │   │
            d───c   h───g
                 ╲ ╱
                  ╳
                 ╱ ╲
            i───j   m───n
            │   │   │   │
            l───k───p───o
            """

        And the ways
            | nodes | highway |
            | abcda | primary |
            | efghe | primary |
            | ijkli | primary |
            | nmop  | primary |
            | cm    | primary |
            | hj    | primary |
            | kp    | primary |

        When I route I should get
            | from | to | route                                 | time   |
            | a    | b  | abcda,abcda                           | 20s    |
            | a    | f  | abcda,cm,nmop,kp,ijkli,hj,efghe,efghe | 257.7s |
            | c    | m  | cm,cm                                 | 44.7s  |

    Scenario: Testbot - Multi level routing: horizontal road
        Given the node map
            """
            a───b   c───d
            │   │   │   │
            e───f   g───h
            │           │
            i═══j═══k═══l
            │           │
            m───n   o───p
            │   │   │   │
            q───r───s───t
            """

        And the ways
            | nodes | highway   |
            | abfea | primary   |
            | cdhgc | primary   |
            | mnrqm | primary   |
            | optso | primary   |
            | ijkl  | primary   |
            | eim   | primary   |
            | hlp   | primary   |
            | rs    | secondary |

        And the data has been extracted
        When I run "osrm-partition --min-cell-size 4 --small-component-size 1 {processed_file}"
        Then it should exit successfully
        And stdout should not contain "level 1 #cells 1 bit size 1"

        When I route I should get
            | from | to | route                          | time |
            | a    | b  | abfea,abfea                    | 20s  |
            | a    | e  | abfea,abfea                    | 20s  |
            | a    | q  | abfea,eim,mnrqm,mnrqm          | 80s  |
            | a    | l  | abfea,eim,ijkl,ijkl            | 100s |
            | a    | t  | abfea,eim,ijkl,hlp,optso,optso | 140s |
            | a    | d  | abfea,eim,ijkl,hlp,cdhgc,cdhgc | 140s |
