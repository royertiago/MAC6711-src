#!/usr/bin/gawk -f
BEGIN {
    count = 0
}

/Test/ {
    delete values;
    value_index = 0
}

/^$/ {
    n = asort(values)
    sum = 0
    for( i = 2; i <= n-1; i++ )
        sum += values[i]

    printf " & %.1f", sum/(n-2)
    count++

    if( count == 4 ) {
        print ""
        count = 0
    }
}

/Run/ {
    val = gensub(/.*: (.*)ms/,"\\1", "g")
    values[value_index] = val
    value_index++
}
