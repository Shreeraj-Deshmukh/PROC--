// Order matters: composite patterns before their substrings
const MAPPINGS = [
    [/\bbasesalary\b/g,  'main'],
    [/\bprocint\b/g,     'int'],
    [/\bprocfloat\b/g,   'float'],
    [/\bprocdouble\b/g,  'double'],
    [/\bprocstring\b/g,  'string'],
    [/\bprocvoid\b/g,    'void'],
    [/\bprocauto\b/g,    'auto'],
    [/\bprocprint\b/g,   'cout'],
    [/\bprocin\b/g,      'cin'],
    [/\bprocreturn\b/g,  'return'],
    [/\bprocfor\b/g,     'for'],
    [/\bprocloop\b/g,    'while'],
    [/\bprocif\b/g,      'if'],
    [/\bprocelse\b/g,    'else'],
    [/\bprocase\b/g,     'case'],
    [/\bprocbreak\b/g,   'break'],
    [/\bproclife\b/g,    'continue'],
    [/\bproclass\b/g,    'class'],
    [/\bproctrue\b/g,    'true'],
    [/\bprocfalse\b/g,   'false'],
    [/\bprocvate\b/g,    'private'],
    [/\bprocted\b/g,     'protected'],
    [/\bfire\b/g,        'delete'],
    [/\bhire\b/g,        'new'],
    [/\bpromote\b/g,     'push_back'],
    [/\bzs\b/g,          'switch'],
    [/\bnes\b/g,         'bool'],
    [/\bvacant\b/g,      'nullptr'],
    [/\bmemo\b/g,        'struct'],
    [/\bdisclosed\b/g,   'public'],
    [/\bdepartment\b/g,  'namespace'],
    [/\bblueprint\b/g,   'template'],
    [/\battempt\b/g,     'try'],
    [/\bescalate\b/g,    'catch'],
    [/\breport\b/g,      'throw'],
    [/\broster\b/g,      'vector'],
    [/\bsalary\b/g,      'const'],
    [/\bbase\b/g,        'static'],
    [/#onboard\b/g,      '#include'],
];

const CTRL = ['procfor', 'procloop', 'procif', 'procelse', 'zs', 'attempt', 'escalate'];

function needsSemicolon(line) {
    const t = line.trimEnd();
    if (!t || t[0] === '#' || t.trimStart().startsWith('//')) return false;
    const last = t[t.length - 1];
    if ([';', '{', '}', ':', ','].includes(last)) return false;
    if (last === ')') {
        const trimmed = t.trimStart();
        if (CTRL.some(kw => trimmed.startsWith(kw))) return false;
        if (t.includes('.')) return true;
        if (trimmed.startsWith('procreturn')) return true;
        return false;
    }
    return /[\w"'\]]$/.test(t);
}

function preprocess(source) {
    return source.split('\n').map(line => {
        line = line.replace(/\bprocprint\((.+)\)/, (_, expr) => `cout << ${expr} << "\\n";`);
        line = line.replace(/\bprocin\((\w+)\)/,   (_, v)    => `cin >> ${v};`);
        if (needsSemicolon(line)) line = line.trimEnd() + ';';
        return line;
    }).join('\n');
}

function transpile(source) {
    let result = preprocess(source);
    for (const [pattern, replacement] of MAPPINGS)
        result = result.replace(pattern, replacement);
    return result;
}
