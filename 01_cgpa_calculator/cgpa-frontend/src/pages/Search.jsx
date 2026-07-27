import { useState } from 'react';
import { api } from '../api';

export default function Search() {
  const [query,   setQuery]   = useState('');
  const [result,  setResult]  = useState(null);
  const [error,   setError]   = useState('');
  const [loading, setLoading] = useState(false);

  const handleSearch = async () => {
    if (!query.trim()) return;
    setError(''); setResult(null); setLoading(true);
    try {
      const res = await api.searchStudent(query.trim());
      setResult(res.data);
    } catch (e) {
      setError(e.response?.status === 404
        ? `Aucun étudiant avec l'ID "${query}".`
        : "Erreur lors de la recherche.");
    } finally {
      setLoading(false);
    }
  };

  const cgpaColor = (v) => v >= 3.5 ? '#22c55e' : v >= 2.5 ? '#f59e0b' : '#ef4444';

  return (
    <div>
      <h1 className="page-title">Rechercher</h1>

      <div className="search-bar">
        <input className="input" placeholder="Entrez un ID étudiant (ex: S001)"
          value={query} onChange={e => setQuery(e.target.value)}
          onKeyDown={e => e.key === 'Enter' && handleSearch()} />
        <button className="btn-primary" onClick={handleSearch} disabled={loading}>
          {loading ? '…' : 'Rechercher'}
        </button>
      </div>

      {error && <div className="error-banner">{error}</div>}

      {result && (
        <div className="detail-panel">
          <div className="detail-header">
            <h2>{result.name} <span className="id-tag">{result.id}</span></h2>
            <span className="cgpa-big" style={{ color: cgpaColor(result.cgpa) }}>
              CGPA {result.cgpa.toFixed(2)}
            </span>
          </div>
          {result.semesters.map(sem => (
            <div key={sem.label} className="sem-block">
              <div className="sem-header">
                <strong>{sem.label}</strong>
                <span>GPA : {sem.gpa.toFixed(2)} | Crédits : {sem.totalCredits}</span>
              </div>
              <table className="table">
                <thead><tr><th>Cours</th><th>Crédits</th><th>Note</th><th>Points qualité</th></tr></thead>
                <tbody>
                  {sem.courses.map(c => (
                    <tr key={c.name}>
                      <td>{c.name}</td>
                      <td>{c.credits}</td>
                      <td><code>{c.letterGrade}</code></td>
                      <td>{c.qualityPts.toFixed(1)}</td>
                    </tr>
                  ))}
                </tbody>
              </table>
            </div>
          ))}
        </div>
      )}
    </div>
  );
}
