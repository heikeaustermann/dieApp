# ifndef SCHLUESSELWOERTER_H
# define SCHLUESSELWOERTER_H

enum class Erzeugungsmethode {
    APPDATA,
    DNS,
    UNDEFINED
};

enum class Ergaenzungsmodus {
    PCAPSTEGO,
    ID2T,
    UNDEFINED
};

enum class Rauschenbetroffene {
    FROMGROUPTOGROUP, 
    FROMALLTOGROUP, 
    FROMGROUPTOALL, 
    FROMALLTOALL, 
    UNDEFINED
};

enum class Aenderungsmodus {
    DELETE,
    SHIFT,
    COPY,
    UNDEFINED
};


# endif